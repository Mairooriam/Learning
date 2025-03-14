(*commenbt *)

TYPE
	New_Enumeration : 
		( (*Enumcommet *)
		member1 := 1, (*Member commet*)
		member2 := 2,
		member3 := 3
		);
	struct2 : 	STRUCT 
		New_Member : USINT; (*struct comment*)
		New_Member1 : USINT;
	END_STRUCT;
	penispumppu2 : 	STRUCT 
		WATER_PUMP_EDM2 : BOOL := TRUE; (*AF103.Di.01*)
		VACUUM_PUMP_EDM2 : BOOL := FALSE; (*AF103.Di.02*)
		WATER_PUMP_OVERLOAD2 : BOOL; (*AF103.Di.04*)
	END_STRUCT;
END_TYPE

(*Insert your comment here.*)

TYPE
	New_Datatype : 	STRUCT  (*datatype comment*)
		New_Member3 : USINT; (*member comment*)
		New_Member2 : USINT;
		New_Member1 : USINT;
		New_Member : USINT;
	END_STRUCT;
END_TYPE
