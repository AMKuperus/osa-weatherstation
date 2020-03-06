package main

import (
	"fmt"
)

func main() {

	// TODO add config file and/or commandline options for flexible configuration
	var m message
	var stationID = 102 // location measurement equipment

	osa := newConnectionOsa("http://localhost", "8180")
	conDb := newConnectionDb("localhost", "3306", "pi", "framboos", "weather")

	db := conDb.open()

	for {
		m = osa.weatherStation(60)
		fmt.Println("Read at : ", m)
		WriteMessage(m, db, stationID)

	}

}
