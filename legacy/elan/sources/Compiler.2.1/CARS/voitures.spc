specification voitures

Vars
	x

Ops
	Corvette:0
	Elegant:1
	High_Mileage:1
	Speed:1
	Body_Shape:1
	Look:1
	Aerodynamic:1
	Color_Choices:1
	Low_Cost:1
	Honda_Civic:0
	Comfortable:1
	Spacious:1
	Cadillac:0
	Extra_Opt:1
	Price_Less_Than_7000:1
	Most_Beautiful:2

Faits

	Elegant(Cadillac)
	Aerodynamic(Corvette)
	Color_Choices(Honda_Civic)
	Most_Beautiful(Cadillac,Corvette)
	High_Mileage(Honda_Civic)
	Speed(Corvette)
	nil

Rules

	Aerodynamic(x)->Body_Shape(x)
	Color_Choices(x)->Look(x)
	Most_Beautiful(Cadillac,x)->Look(x)
	Price_Less_Than_7000(x)->Low_Cost(x)&Most_Beautiful(Corvette,x)
	Spacious(x)&Extra_Opt(x)->Comfortable(x)
	Most_Beautiful(Cadillac,x)&Look(x)->Aerodynamic(x)&Price_Less_Than_7000(x)
	nil

end of specification

