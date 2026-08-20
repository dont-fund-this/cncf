module Trip where

import Type

trip :: [Triplet]
trip =
  [ Triplet "/version" "{}" "{\"once\":true}"
  , Triplet "/storage" "{}" "{\"once\":true}"
  , Triplet "sql.help" "{}" "{\"once\":true}"
  ]
