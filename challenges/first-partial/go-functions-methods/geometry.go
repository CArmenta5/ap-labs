// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct {
	x, y float64
}

func (p *Point) X() float64 {
	return p.x
}
func (p *Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

func segmentIntersect(p1, p2, p3 Point) float64 {
	resu := (p2.Y()-p1.Y())*(p3.X()-p2.X()) - (p3.Y()-p2.Y())*(p2.X()-p1.X())

	return resu
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

func (p *Point) fixPoint() {
	min := -100
	max := 100
	rand.Seed(time.Now().UnixNano())
	p.x = float64(rand.Intn(max-min+1) + min)
	rand.Seed(time.Now().UnixNano())
	p.y = float64(rand.Intn(max-min+1) + min)
}

func collinearVerif(p1 Point, p2 Point, p3 Point) bool {
	ab := math.Hypot(p2.X()-p1.X(), p2.Y()-p1.Y())
	bc := math.Hypot(p3.X()-p2.X(), p3.Y()-p2.Y())
	ac := math.Hypot(p3.X()-p1.X(), p3.Y()-p1.Y())

	if ab+bc == ac {
		return true
	}
	return false
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func main() {
	defer func() {
		if err := recover(); err != nil {
			if len(os.Args) == 1 {
				fmt.Println("No se ingreso ningun parametro -> ", err)
			}
		}
	}()

	flagDen := false
	sides, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Printf("Se envió un parametro no permitido.\n")
	} else if sides < 3 {
		fmt.Printf("Escribre como parametro un numero mayor o igual a 3.\n")
		flagDen = true
	} else if len(os.Args) != 2 {
		fmt.Printf("Favor de escribir solamente el numero de lados <<sides>>.\n")
		flagDen = true
	}

	if err == nil && flagDen == false {
		min := -100
		max := 100
		path := Path{}
		// Rellena los puntos
		for i := 0; i < sides; i++ {
			rand.Seed(time.Now().UnixNano())
			x := float64(rand.Intn(max-min+1) + min)
			rand.Seed(time.Now().UnixNano())
			y := float64(rand.Intn(max-min+1) + min)
			p := Point{x, y}
			path = append(path, p)
		}
		exFlag := false
		for i := 0; i < sides || exFlag; i++ {
			if exFlag{
				i = 0
			}
			first := true
			last := false
			exFlag = false
			for j := i + 1; j < sides; j++ {
				flag := true
				if j == sides-1 {
					last = true
				}
				for first || last {
					if collinearVerif(path[i], path[(i+1)%sides], path[(j+1)%sides]) {
						path[(j+1)%sides].fixPoint()
						i = 0
						exFlag = true
					} else {
						first = false
						last = false
					}
				}
				if i == sides-1 && exFlag == true {
					break
				}
				for flag && j-i > 1 {
					o1 := segmentIntersect(path[i], path[(i+1)%sides], path[j])
					if o1 > 0 {
						o1 = 1
					} else {
						o1 = -1
					}
					o2 := segmentIntersect(path[i], path[(i+1)%sides], path[(j+1)%sides])
					if o2 > 0 {
						o2 = 1
					} else {
						o2 = -1
					}
					o3 := segmentIntersect(path[j], path[(j+1)%sides], path[i])
					if o3 > 0 {
						o3 = 1
					} else {
						o3 = -1
					}
					o4 := segmentIntersect(path[j], path[(j+1)%sides], path[(i+1)%sides])
					if o4 > 0 {
						o4 = 1
					} else {
						o4 = -1
					}
					if o1 != o2 && o3 != o4 {
						if j == sides-1 {
							path[(j+1)%sides].fixPoint()
						} else {
							path[(j+1)%sides].fixPoint()
						}
						i = 0
						exFlag = true
					} else {
						flag = false
					}
				}
			}
		}
		fmt.Printf("Generating a [%d] sides figure\n", sides)
		fmt.Printf("Figure's vertices\n")
		for i := 0; i < sides; i++ {
			fmt.Printf("(  %g,  %g)\n", path[i].x, path[i].y)
		}
		fmt.Printf("Figure's Perimeter\n")
		for i := 0; i < sides; i++ {
			if i < sides-1 {
				fmt.Printf("%.2f  + ", math.Floor(path[i].Distance(path[(i+1)%sides])*100)/100)
			} else {
				fmt.Printf("%.2f = ", math.Floor(path[sides-1].Distance(path[0])*100)/100)
			}
		}
		total := math.Floor(path.Distance()*100)/100 + math.Floor(path[sides-1].Distance(path[0])*100)/100
		fmt.Printf("%.2f\n", total)
	}
}

//!-path
