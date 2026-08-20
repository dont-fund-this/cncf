{-# LANGUAGE ForeignFunctionInterface #-}
module Bind where

import Type
import System.Posix.DynamicLinker
import System.FilePath
import Foreign.Ptr
import Control.Exception (try, SomeException)

foreign import ccall "dynamic"
  mkPump :: FunPtr PumpFn -> PumpFn

bind :: FilePath -> IO (Maybe Cabi)
bind binaryPath = do
  let filename = takeFileName binaryPath
  let skips = ["c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample"]
  if filename `elem` skips
    then return Nothing
    else do
      res <- try (dlopen binaryPath [RTLD_LAZY, RTLD_LOCAL]) :: IO (Either SomeException DL)
      case res of
        Left _ -> return Nothing
        Right dl -> do
          symRes <- try (dlsym dl "Pump") :: IO (Either SomeException (FunPtr PumpFn))
          case symRes of
            Left _ -> return Nothing
            Right symPtr ->
              if symPtr == nullFunPtr
                then return Nothing
                else do
                  let pumpFn = mkPump symPtr
                  return $ Just Cabi
                    { name = filename
                    , path = binaryPath
                    , dylib = dl
                    , pump = pumpFn
                    }
