package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/go-sql-driver/mysql"
)

type conDb struct {
	host   string
	port   string
	user   string
	passwd string
	db     string
}

func newConnectionDb(host, port, user, passwd, db string) conDb {
	var c conDb
	c.host = host
	c.port = port
	c.user = user
	c.passwd = passwd
	c.db = db

	return c
}

func (c *conDb) open() *sql.DB {

	login := fmt.Sprintf("%v:%v@tcp(%v:%v)/%v", c.user, c.passwd, c.host, c.port, c.db)
	fmt.Println(login)
	db, err := sql.Open("mysql", login)
	if err != nil {
		log.Fatal("error opening database", err.Error())
	}
	//defer db.Close() // where to put this one? not importen program at the moment only exit than colsed automaticly
	return db
}

// WriteMessage write the data collected from the weather station to the database
func WriteMessage(m message, db *sql.DB, ID int) {

	ins := fmt.Sprintf("INSERT INTO Measurements ( Temperature, Pressure,Humidity, ID_Station ) VALUES( %f, %f, %f, %d )", m.Temperature, m.Pressure, m.Humidity, ID)
	log.Println("insert record: ", ins)

	_, err := db.Query(ins)

	if err != nil {
		log.Println("error reading SQL: ", err)
	}
}
