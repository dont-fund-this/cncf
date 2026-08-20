module Type where

import Foreign.Ptr
import Foreign.C.String
import Foreign.C.Types
import System.Posix.DynamicLinker

type Address = CString
type Payload = CString
type Options = CString

type PumpFn = Address -> Payload -> Options -> IO CInt

data Cabi = Cabi
  { name :: String
  , path :: String
  , dylib :: DL
  , pump :: PumpFn
  }

data Triplet = Triplet
  { address :: String
  , payload :: String
  , options :: String
  }
