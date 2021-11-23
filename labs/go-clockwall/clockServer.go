// Clock Server is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"

	"time"
)

func TimeIn(t time.Time, name string) (time.Time, error) {
	loc, err := time.LoadLocation(name)
	if err == nil {
		t = t.In(loc)
	}
	return t, err
}

func handleConn(c net.Conn) {
	defer c.Close()
	// fmt.Println("TZ:", os.Getenv("TZ"))
	tz := os.Getenv("TZ")

	for {
		t, errT := TimeIn(time.Now(), tz)
		if errT != nil {
			fmt.Println(tz, "<time unknown>")
		}
		_, err := io.WriteString(c, tz+"..."+t.Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	var nFlag = flag.String("port", "8080", "Write the port to be used")
	flag.Parse()

	listener, err := net.Listen("tcp", "localhost:"+*nFlag)
	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}
