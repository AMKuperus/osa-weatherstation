package main

import (
	"encoding/json"
	"io/ioutil"
	"log"
	"net/http"
	"time"
)

type message struct {
	Temperature float64 `json:"temperature"`
	Pressure    float64 `json:"pressure"`
	Humidity    float64 `json:"humidity"`
}

type conOsa struct {
	url  string
	port string
}

func newConnectionOsa(url, port string) conOsa {
	var c conOsa
	c.url = url
	c.port = port

	return c
}

func (c *conOsa) weatherStation(delay time.Duration) message {

	var message message
	var client http.Client

	time.Sleep(delay * time.Second)

	req, err := http.NewRequest("GET", c.url+":"+c.port, nil)
	if err != nil {
		log.Fatal("error in request url", err)
	}

	resp, err := client.Do(req)
	if err != nil {
		log.Fatal("error request", err)
	}

	if resp.StatusCode != http.StatusOK {
		log.Fatal("error Starus code ", resp.StatusCode)
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

	return message
}
