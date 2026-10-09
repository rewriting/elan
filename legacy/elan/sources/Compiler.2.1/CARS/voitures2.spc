specification voitures2

Vars
	x

Ops
	Corvette:0
	Honda_Civic:0
	Cadillac:0
	Price_Less_Than_7000:1
	Price_Greater_Than_7000:1
	Air_Cond:1
	Power_Window:1
	Sun_Roof:1
	Cassette:1
	Color_Choices:1
	Aerodynamic:1
	Spacious:1
	Elegant:1
	High_Mileage:1
	Speed:1
	Luxury_Car:1
	Extra_Opt:1
	Low_Cost:1
	High_Cost:1
	Body_Shape:1
	Look:1
	Comfortable:1
	Import_Tax:1
	Import_Car:1
	Export_Car:1
	Economical_Car:1
	Performance:1
	Sport_Car:1
	Four_Seats:1
	Expensive_Car:1
	High_Insurance:1
	Car:1

Faits

	Price_Less_Than_7000(Honda_Civic)
	Price_Greater_Than_7000(Corvette)
	Price_Greater_Than_7000(Cadillac)
	Air_Cond(Cadillac)
	Air_Cond(Corvette)
	Power_Window(Cadillac)
	Power_Window(Corvette)
	Sun_Roof(Cadillac)
	Sun_Roof(Corvette)
	Cassette(Cadillac)
	Cassette(Corvette)
	Color_Choices(Cadillac)
	Color_Choices(Honda_Civic)
	Color_Choices(Corvette)
	Aerodynamic(Corvette)
	Aerodynamic(Honda_Civic)
	Spacious(Cadillac)
	Spacious(Honda_Civic)
	Elegant(Cadillac)
	Elegant(Corvette)
	High_Mileage(Honda_Civic)
	Speed(Corvette)
	nil

Rules

	Aerodynamic(x)->Body_Shape(x)
	Color_Choices(x)->Look(x)
	Luxury_Car(x)->Extra_Opt(x)
	Price_Less_Than_7000(x)->Low_Cost(x)
	Price_Greater_Than_7000(x)->High_Cost(x)
	Spacious(x)->High_Cost(x)
	Air_Cond(x)&Power_Window(x)&Sun_Roof(x)&Cassette(x)->Extra_Opt(x)
	Elegant(x)&Body_Shape(x)&Color_Choices(x)->Look(x)
	Spacious(x)&Extra_Opt(x)->Comfortable(x)
	High_Cost(x)->Import_Tax(x)
	Import_Tax(x)->Import_Car(x)
	Import_Tax(x)->Export_Car(x)
	Import_Car(x)->Extra_Opt(x)
	Look(x)&Comfortable(x)&High_Cost(x)->Luxury_Car(x)
	High_Mileage(x)&Low_Cost(x)->Economical_Car(x)
	Look(x)&High_Cost(x)&Speed(x)&Performance(x)->Sport_Car(x)
	Spacious(x)->Four_Seats(x)
	Expensive_Car(x)->High_Insurance(x)
	Luxury_Car(x)->Car(x)
	Sport_Car(x)->Car(x)
	Economical_Car(x)->Car(x)
	nil

end of specification