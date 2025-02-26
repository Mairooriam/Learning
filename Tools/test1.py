from dataclasses import dataclass
from typing import Optional

.typ

# kommentteja
# tyyppejä
## ENum
## struct

# Kommentti -> type -> #kommentti 


dicti["kommentit"] = {(*mitä vaan*)}
icti["New_Enumeration"]   (dasd := 1, (*comm*) dsadsa := 2 (*commmm*));
 dicti[CC1_Do_Typ]

examplefile = '''
(*mitä vaan*) ->> 19
TYPE
    New_Enumeration : ->> 21 
        (
        dasd := 1, (*comm*)
        dsadsa := 2 (*commmm*)
        );
    CC1_Do_Typ : 	STRUCT --> 26
        
        WaterPumpMCT : CC1_Di_Typ; (*AF102.Do.02*)
        VacyymPumpMCT : BOOL; (*AF102.Do.03*)
        WaterFillValve : BOOL; (*AF102.Do.04*)
        SequenceOnLED : BOOL; (*AF102.Do.05*)
        NAOH_PUMP_CONTROL : BOOL; (*AF102.Do.07*)
        LIGHT_TOWER_SCRUBBER_OFF : BOOL; (*AF102.Do.08*)
        LIGHT_TOWER_VACUUM_OFF : BOOL; (*AF102.Do.09*)
        LIGHT_TOWER_CHECK_pH : BOOL; (*AF102.Do.10*)
        LIGHT_TOWER_WATER_LEVEL_LOW : BOOL; (*AF102.Do.11*)
        LIGHT_TOWER_ALARM_SOUND_ON : BOOL; (*AF102.Do.12*)
    END_STRUCT;
END_TYPE
(*COMMENT*) -->> 40
TYPE
    New_Enumeration : 
        (
        dasd := 1, (*comm*)
        dsadsa := 2 (*commmm*)
        );
    CC1_Do_Typ : 	STRUCT 
        
        WaterPumpMCT : CC1_Di_Typ; (*AF102.Do.02*)
        VacyymPumpMCT : BOOL; (*AF102.Do.03*)
        WaterFillValve : BOOL; (*AF102.Do.04*)
        SequenceOnLED : BOOL; (*AF102.Do.05*)
        NAOH_PUMP_CONTROL : BOOL; (*AF102.Do.07*)
        LIGHT_TOWER_SCRUBBER_OFF : BOOL; (*AF102.Do.08*)
        LIGHT_TOWER_VACUUM_OFF : BOOL; (*AF102.Do.09*)
        LIGHT_TOWER_CHECK_pH : BOOL; (*AF102.Do.10*)
        LIGHT_TOWER_WATER_LEVEL_LOW : BOOL; (*AF102.Do.11*)
        LIGHT_TOWER_ALARM_SOUND_ON : BOOL; (*AF102.Do.12*)
    END_STRUCT;
END_TYPE
'''

# 


@dataclass
class CC1_Do_Typ:
    WaterPumpMCT: bool
    VacyymPumpMCT: bool
    WaterFillValve: bool
    SequenceOnLED: bool
    NAOH_PUMP_CONTROL: bool
    LIGHT_TOWER_SCRUBBER_OFF: bool
    LIGHT_TOWER_VACUUM_OFF: bool
    LIGHT_TOWER_CHECK_pH: bool
    LIGHT_TOWER_WATER_LEVEL_LOW: bool
    LIGHT_TOWER_ALARM_SOUND_ON: bool

# Example usage
struct = CC1_Do_Typ(
    WaterPumpMCT=False,
    VacyymPumpMCT=False,
    WaterFillValve=False,
    SequenceOnLED=False,
    NAOH_PUMP_CONTROL=False,
    LIGHT_TOWER_SCRUBBER_OFF=False,
    LIGHT_TOWER_VACUUM_OFF=False,
    LIGHT_TOWER_CHECK_pH=False,
    LIGHT_TOWER_WATER_LEVEL_LOW=False,
    LIGHT_TOWER_ALARM_SOUND_ON=False
)

struct.WaterFillValve

print(struct)
