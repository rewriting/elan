{{BOROGRAMMAR}}
extension ldmodgram;
typeuse logicdescription,specaxioms,handbypart,specaxiom,axiomname,
	logicname,queryhandbypart,qresult,qstartwith,allaxioms,
        qcheckwith,mstratname,sortpart,qhandbypart,axiomsbegin,RWglob,RWloc,
        mimodule,
	imodule,actarglist,
	Imodule,Actarglist,
	checkwith,imodulelist;
op
  LPL __ description
    __
    __
    end
    query __
  end
  ::(logicname,axiomsbegin,allaxioms,queryhandbypart)logicdescription	code 3;

  LPL __ description
    query __
  end
  ::(logicname,queryhandbypart)logicdescription		code 3;

    
  specification description :: axiomsbegin		code 15;


  __	::(identifier)logicname			code 2;

  __	::(specaxioms)allaxioms			code 5;
  __	::(specaxiom)specaxioms;
  __ __ ::(specaxioms,specaxiom)specaxioms;

  part __ __ __		::(axiomname,sortpart,handbypart)specaxiom    code 20;
  part __ __ __ __	::(axiomname,sortpart,handbypart,checkwith)specaxiom;

  __	::(identifier)axiomname			code 1;
{{  of sort __	:: (imodule)sortpart		code 13;}}
  of sort __	:: (Imodule)sortpart		code 13;
  import __ 	::(imodulelist)handbypart;
  global	:: RWglob                    code 21;
  local		:: RWloc                     code 22; 
  import __ __ 	::(RWglob,imodulelist) handbypart;
  import __ __ 	::(RWloc,imodulelist) handbypart;
  import __ __ __ __ 	::(RWglob,imodulelist,RWloc,imodulelist) handbypart;
  check with	::checkwith		  code 19;

  __ __ __ __     ::(sortpart,qresult,qhandbypart,qstartwith)
		  	queryhandbypart code 6;

  __ __ __ __ __  ::(sortpart,qresult,qhandbypart,qcheckwith,qstartwith)
			queryhandbypart code 7;


 __ 	::(Imodule) imodulelist			code 4;
 __ __  ::(imodulelist,Imodule) imodulelist	code 4;

{{
 __ 	::(imodule) imodulelist			code 4;
 __ __  ::(imodulelist,imodule) imodulelist	code 4;
}}
 __	::(number)mimodule			code 16;  {{ ... SYMBS}}
 __	::(identifier)mimodule			code 16;

{{--imodule--}}
 __	::(mimodule)imodule;
 __[__] ::(mimodule,actarglist)imodule		code 17;

 __	::(imodule)actarglist;
 __,__  ::(actarglist,imodule)actarglist	code 18;
 __|__  ::(actarglist,imodule)actarglist	code 18;

{{--Imodule--}}
 __	::(mimodule)Imodule;
 __[__] ::(mimodule,Actarglist)Imodule		code 17;

 < __ >       :: (Imodule) Imodule		code 23;
 < __ -> __ > :: (Imodule,Imodule) Imodule	code 24;

 __	::(Imodule)Actarglist;
 __,__  ::(Actarglist,Imodule)Actarglist	code 18;
 __|__  ::(Actarglist,Imodule)Actarglist	code 18;

  result of sort __	::(Imodule) qresult		code 9;
{{  result of sort __	::(imodule) qresult		code 9;}}
  start with (__)	::(mstratname) qstartwith 	code 10;
  start with ()		:: qstartwith 			code 10;
{{BEGINBORO}}
  start with [		::qstartwith 			code 14;
{{ENDBORO}}
  check with		::qcheckwith		  	code 11;
  import __		::(imodulelist)qhandbypart;

  import __ __ 	::(RWglob,imodulelist)qhandbypart;
  import __ __ 	::(RWloc,imodulelist)qhandbypart;
  import __ __ __ __	::(RWglob,imodulelist,RWloc,imodulelist)qhandbypart;
  __			::(identifier) mstratname 	code 12;

::
axiom
for logicdescription:

end
