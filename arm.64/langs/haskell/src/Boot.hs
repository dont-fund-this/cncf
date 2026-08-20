module Boot where

import Type
import Find
import Bind
import System.Environment
import Data.Maybe (catMaybes)

boot :: Maybe String -> IO [Cabi]
boot targetDir = do
  envLib <- lookupEnv "PAT_LIB"
  case envLib of
    Just lib | not (null lib) -> do
      m <- bind lib
      case m of
        Just c -> return [c]
        Nothing -> return []
    _ -> do
      files <- Find.find targetDir
      results <- mapM Bind.bind files
      return (catMaybes results)
