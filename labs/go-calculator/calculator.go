package main

import (
	"fmt"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {
	res := values[0]
	fmt.Printf("%v ", res)
	switch operator {
	case 1:
		for i := 1; i < len(values); i++ {
			fmt.Printf("+ %v ", values[i])
			res += values[i]
		}
	case 2:
		for i := 1; i < len(values); i++ {
			fmt.Printf("- %v ", values[i])
			res -= values[i]
		}
	case 3:
		res = 1
		for i := 1; i < len(values); i++ {
			fmt.Printf("* %v ", values[i])
			res *= values[i]
		}
	}
	fmt.Printf("= ")
	return res

}

// Convierte la operacion string a su representación del programa
// 1-3 exitosa
// -1 no exitosa
func designateNumber(operation string) int {
	switch operation {
	case "add":
		return 1
	case "sub":
		return 2
	case "mult":
		return 3
	default:
		return -1
	}
}

// Convierte el array de strings a ints y regresa un arreglo, y bool en caso que sea exitosa
func allNumbers(array []string) (bool, []int) {
	newArray := make([]int, len(array))
	if len(array) <= 1 {
		fmt.Printf("No se mandó el arreglo correspondiente correctamente.\n")
		return false, newArray
	}
	for i := 0; i < len(array); i++ {
		val, err := strconv.Atoi(array[i])
		if err != nil {
			fmt.Printf("Se ha mandando un arreglo que no contiene puros enteros.\n")
			return false, newArray
		} else {
			newArray[i] = val
		}
	}
	return true, newArray
}

func main() {
	defer func() {
		if err := recover(); err != nil {
			if len(os.Args) <= 3 {
				fmt.Printf("Error (No se mandaron los suficientes parametros para el programa) -> %v \n", err)
			}
		}
	}()

	operator := designateNumber(os.Args[1])
	if operator == -1 {
		fmt.Print("No se mandó ningun operador disponible.\n")
	}

	flag, arrNumbers := allNumbers(os.Args[2:])
	if flag && operator != -1 {
		res := calc(operator, arrNumbers)
		fmt.Printf("%v \n", res)
	}
}
