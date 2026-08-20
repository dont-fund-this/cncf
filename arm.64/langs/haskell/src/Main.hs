module Main where

import Boot
import Trip
import Type
import System.Environment
import Foreign.C.String
import Text.Printf

main :: IO ()
main = do
  args <- getArgs
  let targetDir = if null args then Nothing else Just (head args)
  dist <- Boot.boot targetDir
  mapM_ (\d -> mapM_ (\t ->
    withCString (address t) $ \cA ->
    withCString (payload t) $ \cP ->
    withCString (options t) $ \cO ->
      pump d cA cP cO
    ) Trip.trip) dist
  printf "{\n  \"lang\": \"haskell\",\n  \"status\": \"ready\",\n  \"engines\": %d\n}\n" (length dist)
