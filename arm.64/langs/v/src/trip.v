module main

pub fn trip() []Triplet {
	return [
		Triplet{
			address: '/version'
			payload: '{}'
			options: '{"once":true}'
		},
		Triplet{
			address: '/storage'
			payload: '{}'
			options: '{"once":true}'
		},
		Triplet{
			address: 'sql.help'
			payload: '{}'
			options: '{"once":true}'
		},
	]
}
