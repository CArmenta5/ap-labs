/* Carlos Raúl Armenta Pérez A01635371 */
package main

import (
	"fmt"
	"os"
)

func main() {
	// With recover and defer we can catch the panic and printing out.
	// recover -> catch the panic and return the panic message

	// err != nil (the pointer do not point to anything)
	// func() handle panics that may raise while main is running
	defer func() {
		if err := recover(); err != nil {
			if(len(os.Args) == 1 ){
				fmt.Printf("Error message (There is no parameter to use as name) -> %v \n", err)
			}
		}
	}()
	
	var name string
	argsNames := os.Args[1:]
	tamaño := len(argsNames)
	
	for i := 0; i < tamaño-1; i++ {
		name += argsNames[i]+" "
	}

	name += argsNames[tamaño-1]

	fmt.Printf("Hello %v, Welcome to the Jungle \n", name)
}
