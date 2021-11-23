package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

func main() {

	tzArgs := make(chan []string, len(os.Args)-1)
	done := make(chan bool)

	go func() {
		for {
			foo, ok := <-tzArgs
			// ok false if tzArgs completed
			if ok {

				conn, err := net.Dial("tcp", foo[1])
				if err != nil {

					log.Fatal("Error dentro de la conexión.")
				}
				defer conn.Close()

				tmp := make([]byte, 256)
				for {
					_, err := conn.Read(tmp)
					if err != nil {
						if err != io.EOF {
							fmt.Println("read error:", err)
						}
						break
					}
					//fmt.Println("got", n, "bytes.")
					tz := strings.SplitN(string(tmp), "...", 2)
					print(tz[0] + ": ")
					fmt.Print(tz[1])
					break
				}

			} else {
				done <- true
				return
			}
		}
	}()
	// Send len of args over the tzArgs channel, then it close it
	for i := 1; i < len(os.Args); i++ {
		pair := strings.SplitN(os.Args[i], "=", 2)
		tzArgs <- pair
	}
	close(tzArgs)
	<-done

}
