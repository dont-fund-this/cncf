module Find where

import System.Directory
import System.FilePath
import System.Environment
import Control.Monad

find :: Maybe String -> IO [FilePath]
find targetDir = do
  envDir <- lookupEnv "DIST_DIR"
  let candidates = ["dist", "../../dist", "../../../dist"]
  existing <- filterM doesDirectoryExist candidates
  let fallback = if null existing then "dist" else head existing
  let dir = case targetDir of
              Just d -> d
              Nothing -> maybe fallback id envDir
  exists <- doesDirectoryExist dir
  if not exists
    then return []
    else do
      entries <- listDirectory dir
      let valid = filter (/= ".DS_Store") entries
      filterM doesFileExist (map (dir </>) valid)
