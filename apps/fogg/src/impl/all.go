package impl

import (
	_ "fogg/src/impl/api/v1/namespaces"
	namespaces "fogg/src/impl/api/v1/namespaces"
	_ "fogg/src/impl/api/v1/namespaces/serviceaccounts"
	serviceaccounts "fogg/src/impl/api/v1/namespaces/serviceaccounts"
	"fogg/src/impl/version"
	"fogg/src/type"
)

var AllDefs = []types.Def{
	version.VersionGet,
	namespaces.NamespacePost,
	serviceaccounts.ServiceAccountPost,
}

func All() []types.Def {
	return AllDefs
}
