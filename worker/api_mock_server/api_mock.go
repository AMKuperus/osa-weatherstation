package main

import (
	"log"
	"net/http"
)

func main() {
	osaJSON := []byte(`{"Temperature":21.4, "Pressure": 1007.6, "Humidity":43.7}`)

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		w.WriteHeader(http.StatusOK)
		w.Write([]byte(osaJSON))
	})

	log.Fatal(http.ListenAndServe(":8180", nil)) //port 8180 used otherwise get conflict with docker mock db
}
