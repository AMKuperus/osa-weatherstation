package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

type message struct {
	Temperature float64 `json:"temperature"`
	Pressure    float64 `json:"pressure"`
	Humidity    float64 `json:"humidity"`
}

func connect() {

	var message message
	var client http.Client
	var port string

	ipWeaterStation := "localhost"
	port = "8080"

	req, err := http.NewRequest("GET", "http://"+ipWeaterStation+":"+port, nil)
	if err != nil {
		log.Println("Error in request url", err)
	}

	resp, err := client.Do(req)
	if err != nil {
		log.Fatal("Error request", err)
	}

	log.Println("Http status code: ", resp.StatusCode)
	if resp.StatusCode != http.StatusOK {
		log.Println("Error Starus code ", resp.StatusCode)
	} else {
		bBytes, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			log.Println("Body error", err)
		}

		err = json.Unmarshal(bBytes, &message)
		if err != nil {
			log.Println("Body error", err)
		}
	}
	fmt.Println("body: ", message)
	return
}

func main() {
	// ticker := time.NewTicker(1 * time.Second)
	// sigs := make(chan os.Signal, 1)
	// //done := make(chan bool)

	// signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	// go func() {
	// 	for {
	// 		select {
	// 		case <-sigs:
	// 			return
	// 		case <-ticker.C:
	// 			connect()
	// 		}
	// 	}
	// }()
	connect()
	//time.Sleep(5 * time.Second)
	// sigs := make(chan os.Signal, 1)
	// done := make(chan bool, 1)

	// signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	// go func() {
	// 	sig := <-sigs
	// 	fmt.Println()
	// 	fmt.Println(sig)
	// 	done <- true
	// }()
	// fmt.Println("awaiting signal")
	// connect()

	// <-done
	// fmt.Println("exiting")

}
