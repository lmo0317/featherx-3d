--=======================================
-- Lua
-- by Min
--=======================================
-- filename: LuaSupport.lua
-- created: 20071017
-- descrip: SupportFile
--=======================================
function MainEvent(id, eventCode,arg1,arg2,arg3,str)
	if eventCode == EVENT_INIT  then
		--AddModelAse("map.ase",5,0,0,0,0,0,1)
		--AddModelAse("car.ase",5,0,0,0,90,0,0.1)
		AddModelAse("123123.ase",5,0,0,0,90,0,10)
		
		AddMapObject(0,5,100,0)
		--AddPlayer(85,30,100,1)	
		--AddMapObject(0,100,100,0)		 
	elseif eventCode == EVENT_TEST  then
		Test("123")	
	end
end






