package main

func trip() []Triplet {
	return []Triplet{
		{Address: "/version", Payload: "{}", Options: `{"once":true}`},
		{Address: "/storage", Payload: "{}", Options: `{"once":true}`},
		{Address: "sql.help", Payload: "{}", Options: `{"once":true}`},
	}
}
