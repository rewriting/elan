extension modgram;
typeuse ssymbol,sssymbol,ssymbollist,profil,atrib,atribl,
        module,imprest,sortrest,oprest,stratoprest,importlist,sortlist,psortlist,
        opdef,opdef1,opdef2,opdeffalias,opdeflist,rulerest,eofmod,
        rule,rulelist,declare,declarelist,beforebody,trname,rtype,
        beforerightside, afterrightside, afterrightside1, beforeif, 
	beforewhere, beforewhere2,varlist,vartype,beforerwbody,beforewhere3,
        ops,RWglobal,RWlocal,RWexplicit,RWimplicit,
	opdef3,RWopend,normbegin,
	modbegin,strategydesc,strategydesc1,stratname,
	stratbody,strategy,repbegin,iterbegin,iterplusbegin,whilebegin,
	beforerwif,modulename,paramrest,parammodlist,
	first2beg,
	dontcare2beg,dontknow2beg,one2beg,normin,normout,
	dontknowcon2beg,dontcarecon2beg, onecon2beg,
	strlist,symcode,endofstrategies,
        ncbeforebody, ncbeforebody1,
	ncbeforerwbody,ncrule,ncrulelist,rdeclars,
	endofrules, endofrules1,opbegin,
        stratlist,stratrest,mmodulename,farglist,
	actarglist,imodule,Actarglist,Imodule,Imodule2,
	mimodule,
	prname,numofcall,restype,bracketl,imports,RWimport,RWrules,
	varname,iimportlist,inlinebeg,inlinelist,inlinerest,hardaliasdef,
        spidentsymbol,pattype,sbracketl,
       strdeflist, strs, strdef, 
       strdef1, strdeffalias, rest2, RWdefined,
       trname1, varlist1, varname1,
       stratdef,stratdeflist,  RWbody, sdeclars, sdeclarsex,sdeclarsim,
       stype, stratcall,sdeclarsim1,koko,dotname,
       RWstrategies, RWsymbols, RWoperators, RWstratop,
       sntype, strtypedlist,
       Gtypeaux,Gtype, listint, 
       sdeclvars,sdeclsymbs, selector,parrow,
       tallbegin,tonebegin,tsomebegin,rewritebegin;

op

{{ syntax : }}
{{ of module }}

 __ __ __ ::(modbegin,modulename,imprest)module       code 144;

 module			::modbegin;

 __	  ::(identifier)mmodulename			code 169;

 __       ::(mmodulename)modulename			code 189;
 __ [__]  ::(mmodulename,farglist)modulename		code 189;
 
 __ 	  ::(identifier)farglist			code 188;
 __,__    ::(farglist,identifier)farglist		code 188;

 import		::RWimport				code 202;
 __ __ end  __  ::(RWimport,imports,sortrest) imprest;
 __		::(sortrest) imprest;

 sort __ ; end __     ::(sortlist,oprest)sortrest;
 __		::(oprest) sortrest;


 operators	:: RWoperators				code 335;
 stratop	:: RWstratop				code 336;
 end	        :: RWopend				code 208;

 __ __ __ __	:: (RWoperators,ops,RWopend,oprest)oprest;
 __ __ __ __	:: (RWstratop,ops,RWopend,oprest) oprest;
 __ __          :: (rule,oprest)oprest;
 __ __          :: (strategydesc,oprest)oprest;
 __	  	:: (eofmod)oprest;


 end  	  ::eofmod;

{{ of imports }}

 __ ;		::(iimportlist) imports;
 __ __ ;	::(RWglobal,iimportlist) imports;
 __ __ ;	::(RWlocal,iimportlist) imports;
 __ __ ; __ __;	::(RWglobal,iimportlist,RWlocal,iimportlist) imports;

 __	::(importlist)iimportlist		code 201;

 __	::(Imodule) importlist			code 8;
 __ __	::(importlist,Imodule) importlist	code 8;


{{--imodule--}}
 __	::(number)mimodule			code 185;   {{... #ifdef SYMBS}}
 __	::(identifier)mimodule			code 185;

 __	::(mimodule)imodule;
 __[__] ::(mimodule,actarglist)imodule		code 186;

 __	::(imodule)actarglist;
 __,__  ::(actarglist,imodule)actarglist	code 187;
 __|__  ::(actarglist,imodule)actarglist	code 187;


{{--Imodule--}}

 __	::(mimodule)Imodule;
 __[__] ::(mimodule,Actarglist)Imodule		code 186;

 < __ >       :: (Imodule) Imodule		code 172;
 < __ -> __ > :: (Imodule,Imodule) Imodule	code 173;

 __	::(Imodule)Actarglist;
 __,__  ::(Actarglist,Imodule)Actarglist	code 187;
 __|__  ::(Actarglist,Imodule)Actarglist	code 187;



{{ of sorts }}

 __	::(Imodule) sortlist			code 28;
 __ __	::(sortlist,Imodule) sortlist		code 29;


{{ of ops }}

 __ __ __  __ ::(RWglobal,opdeflist,RWlocal,opdeflist)ops;
 __ __ 	      ::(RWglobal,opdeflist)ops;
 __ __ 	      ::(RWlocal,opdeflist)ops;

 global	 ::RWglobal		code 140;
 local	 ::RWlocal		code 141;
 export  ::RWglobal		code 140;
 defined ::RWdefined;
 explicit::RWexplicit;
 implicit::RWimplicit;

 __ 	::(opdef)opdeflist;
 __ __  ::(opdeflist,opdef)opdeflist;


 __	::(atrib)atribl;
 __ __  ::(atribl,atrib)atribl;

 builtin __ 	::(number)atrib			code 145;
 bs __ 		::(number)atrib			code 145;
 builtin  	:: atrib			code 152;
 bs 	 	:: atrib			code 152;
{{SEMANTIC}}
 code __	::(number)atrib			code 148;
 code -__	::(number)atrib			code 149;

 pri __  	::(number)atrib			code 23;
 assocLeft	::atrib				code 198;
 assocRight	::atrib				code 199;
 strategy __    ::(listint) atrib		code 240;
 __		:: (number) listint		code 241;
 __ __		:: (listint, number) listint	code 242;
 definedAs __	::(identifier)atrib		code 204;
 (AC)     	::atrib				code 20;

 __ ;	     	::(opdef1)opdef				code 22;
 __         	::(opdef1)opdeffalias  			code 130;
 __ alias __ :; ::(opdeffalias,ssymbollist) opdef	code 25;
 __  : __ ;	::(hardaliasdef,profil) opdef		code 206;
 __ hardAlias __ ::(opdeffalias,ssymbollist)hardaliasdef	code 207;


 __ 	::(ssymbol)ssymbollist;
 __ __  ::(ssymbollist,ssymbol)ssymbollist;


 __     	::(Imodule)profil			code 18;
 (__)__ 	::(psortlist,Imodule)profil		code 19;

 __	 	:: (identifier) selector		code 369;
 __ : __        :: (selector, Imodule) Imodule2		code 370;

 __		:: (Imodule) psortlist				code 30;
 __		:: (Imodule2) psortlist				code 30;
 __ __		:: (psortlist,Imodule) psortlist		code 31;
 __ __		:: (psortlist,Imodule2) psortlist		code 31;


{{ codes of symbols }}

 __	::(number)symcode			code 183;
 '\__'  ::(symcode)ssymbol			code 182;

  __	::(sssymbol)ssymbol;
  '__'  ::(sssymbol)ssymbol;
  '''   ::ssymbol                               code 39;
  ':'   ::ssymbol                               code 58;
  '@'   ::ssymbol                               code 64;
  @     ::ssymbol                               code 153;
  '__'  ::(spidentsymbol)ssymbol;

         __ ::(identifier)sssymbol		code 26;
         __ ::(number)sssymbol			code 27;
         module	:: sssymbol			code 26;
         import :: sssymbol			code 26;
         sort 	:: sssymbol			code 26;
         operators :: sssymbol			code 26;
         stratop :: sssymbol			code 26;
         AC 	:: sssymbol			code 26;

         global :: sssymbol			code 26;

         public :: sssymbol			code 26;
         for 	:: sssymbol			code 26;
         declare :: sssymbol			code 26;
         strategies :: sssymbol			code 26;
         strategy   :: sssymbol			code 26;

{{MV strategies}}
         repeat :: sssymbol			code 26;
         dc 	:: sssymbol			code 26;
{{ [pem: Oct 26 00] }}
         tall   :: sssymbol			code 26;
         tone   :: sssymbol			code 26;
         tsome  :: sssymbol			code 26;
{{ [pem: Apr  8 02] }}
         rewrite :: sssymbol			code 26;
{{ [Huy: May  4 00] }}
         normin :: sssymbol                      code 26;
         normout :: sssymbol                     code 26;
         first 	:: sssymbol			code 26;
         dk 	:: sssymbol			code 26;
         one	:: sssymbol			code 26;
         dont 	:: sssymbol			code 26;
         care 	:: sssymbol			code 26;
         choose :: sssymbol			code 26;
         builtin:: sssymbol			code 26;
         bs  	:: sssymbol			code 26;
         id 	:: sssymbol			code 26;
         fail 	:: sssymbol			code 26;
         dccall	:: sssymbol			code 26;
         dkcall	:: sssymbol			code 26;
         call	:: sssymbol			code 26;
         know 	:: sssymbol			code 26;
         iterate:: sssymbol			code 26;
         handline:: sssymbol                    code 26;
         query  :: sssymbol                     code 26;
         source :: sssymbol                     code 26;
         of     :: sssymbol                     code 26;
         result :: sssymbol                     code 26;
         queryend:: sssymbol                    code 26;
         start  :: sssymbol                     code 26;
         with   :: sssymbol                     code 26;
         if 	:: sssymbol			code 26;
         where 	:: sssymbol			code 26;

	 case	:: sssymbol			code 26;
	 then	:: sssymbol			code 26;
	 switch	:: sssymbol			code 26;
	 otherwise :: sssymbol			code 26;
         try 	:: sssymbol			code 26;
         alias 	:: sssymbol			code 26;

         Epsilon       :: spidentsymbol		code 26;

         local         :: spidentsymbol		code 26;

	 private       :: spidentsymbol		code 26;
         Epsilon       :: ssymbol		code 210;


         !  :: sssymbol                         code 33;
         "   :: sssymbol                        code 34;
         #  :: sssymbol                         code 35;
         $  :: sssymbol                         code 36;
         %  :: sssymbol                         code 37;
         &  :: sssymbol                         code 38;
         (  :: sssymbol                         code 40;
         )  :: sssymbol                         code 41;
         *  :: sssymbol                         code 42;
         +  :: sssymbol                         code 43;
         ,  :: sssymbol                         code 44;
         -  :: sssymbol                         code 45;
         .  :: sssymbol                         code 46;
         /  :: sssymbol                         code 47;
         ;  :: sssymbol                         code 59;
         <  :: sssymbol                         code 60;
         =  :: sssymbol                         code 61;
         >  :: sssymbol                         code 62;
         ?  :: sssymbol                         code 63;
         [  :: sssymbol                         code 91;
         \  :: sssymbol                         code 92;
         ]  :: sssymbol                         code 93;
         ^  :: sssymbol                         code 94;
         _  :: sssymbol                         code 95;
         `  :: sssymbol                         code 96;
         {  :: sssymbol                         code 123;
         |  :: sssymbol                         code 124;
         }  :: sssymbol                         code 125;
         ~  :: sssymbol                         code 126;


{{ of rules }}

 __     ::(rule) rulelist;
 __ __  ::(rulelist,rule) rulelist;

  __      ::(declarelist)rdeclars; 

 __ for __ __ __ __ __ __ __ 	::
		(RWrules,rtype,rdeclars,RWglobal,ncrulelist,RWlocal,ncrulelist,endofrules)rule;
 __ for __    __ __ __ __ __ 	::
		(RWrules,rtype,         RWglobal,ncrulelist,RWlocal,ncrulelist,endofrules)rule;

 __ for __ __ __ __ __ __ __ 	::
		(RWrules,rtype,rdeclars,RWlocal,ncrulelist,RWglobal,ncrulelist,endofrules)rule;
 __ for __    __ __ __ __ __ 	::
		(RWrules,rtype,         RWlocal,ncrulelist,RWglobal,ncrulelist,endofrules)rule;

 __ for __ __ __ __ __ 		::(RWrules,rtype,rdeclars,RWglobal,ncrulelist,endofrules)rule;
 __ for __    __ __ __ 		::(RWrules,rtype,         RWglobal,ncrulelist,endofrules)rule;

 __ for __ __ __ __ __ 		::(RWrules,rtype,rdeclars,RWlocal,ncrulelist,endofrules)rule;
 __ for __    __ __ __ 		::(RWrules,rtype,         RWlocal,ncrulelist,endofrules)rule;

 __ for __ __ __       		::(RWrules,rtype,rdeclars,endofrules)           rule;


 strategies  		:: RWstrategies	 code 360; 

 [		   :: sbracketl; 
 __ __ ] __ end    :: (sbracketl,stratname,stratbody) stratdef code 167;

 __   	   	   :: (stratdef) stratdeflist;
 __ __  	   :: (stratdeflist,stratdef) stratdeflist;


 {{ -- sans des strategies de Peter --}}

 __ for __ __ __ __ __ __ __ 	::
		(RWstrategies,stype,declarelist,RWglobal,stratdeflist,RWlocal,stratdeflist,endofrules) strategydesc1;
 __ for __    __ __ __ __ __ 	::
		(RWstrategies,stype,         RWglobal,stratdeflist,RWlocal,stratdeflist,endofrules) strategydesc1;

 __ for __ __ __ __ __ __ __ 	::
		(RWstrategies,stype,declarelist,RWlocal,stratdeflist,RWglobal,stratdeflist,endofrules) strategydesc1;
 __ for __    __ __ __ __ __ 	::
		(RWstrategies,stype,         RWlocal,stratdeflist,RWglobal,stratdeflist,endofrules) strategydesc1;


 __ for __ __ __ __ __ 		::(RWstrategies,stype,declarelist,RWglobal,stratdeflist,endofrules) strategydesc1;
 __ for __    __ __ __ 		::(RWstrategies,stype,         RWglobal,stratdeflist,endofrules) strategydesc1;

 __ for __ __ __ __ __ 		::(RWstrategies,stype,declarelist,RWlocal,stratdeflist,endofrules) strategydesc1;
 __ for __    __ __ __ 		::(RWstrategies,stype,         RWlocal,stratdeflist,endofrules) strategydesc1;

 __ for __ __ __       		::(RWstrategies,stype,declarelist,endofrules) strategydesc1;
 __ for __ __        		::(RWstrategies,stype,endofrules) strategydesc1;

 {{ -- avec des strategies de Peter --}}

 __ for __ __ __ __ __ __ __ __ __	::(RWstrategies,stype,declarelist,RWglobal,stratdeflist,RWlocal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;
 __ for __    __ __ __ __ __ __ __	::(RWstrategies,stype,         RWglobal,stratdeflist,RWlocal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;


 __ for __ __ __ __ __ __ __ __ __	::(RWstrategies,stype,declarelist,RWlocal,stratdeflist,RWglobal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;
 __ for __    __ __ __ __ __ __ __	::(RWstrategies,stype,         RWlocal,stratdeflist,RWglobal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;



 __ for __ __ __ __ __ __ __		::(RWstrategies,stype,declarelist,RWglobal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;
 __ for __    __ __ __ __ __		::(RWstrategies,stype,         RWglobal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;

 __ for __ __ __ __ __ __ __		::(RWstrategies,stype,declarelist,RWlocal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;
 __ for __    __ __ __ __ __		::(RWstrategies,stype,         RWlocal,stratdeflist,RWdefined,sdeclars,endofrules) strategydesc1;

 __ for __ __ __ __ __      		::(RWstrategies,stype,declarelist,RWdefined,sdeclars,endofrules) strategydesc1;
 __ for __ __ __ __      		::(RWstrategies,stype            ,RWdefined,sdeclars,endofrules) strategydesc1;

 __	:: (strategydesc1) strategydesc		code 330;


{{syntax de Claude}}
 __ for __ __ __ __ __ __ __ 	::(RWstrategies,stype,declarelist,RWexplicit,sdeclarsex,RWimplicit,sdeclarsim,endofrules) strategydesc;
 __ for __    __ __ __ __ __ 	::(RWstrategies,stype,         RWexplicit,sdeclarsex,RWimplicit,sdeclarsim,endofrules) strategydesc;

 __ for __ __ __ __ __ 		::(RWstrategies,stype,declarelist,RWexplicit,sdeclarsex,endofrules) strategydesc;
 __ for __    __ __ __ 		::(RWstrategies,stype,         RWexplicit,sdeclarsex,endofrules) strategydesc;

 __ for __ __ __ __ __ 		::(RWstrategies,stype,declarelist,RWimplicit,sdeclarsim,endofrules) strategydesc;
 __ for __    __ __ __ 		::(RWstrategies,stype,         RWimplicit,sdeclarsim,endofrules) strategydesc;
{{end of syntax de Claude}}

 [			::bracketl			code 143;
 __ __ ]		::(bracketl,trname)ncbeforebody code 155;
 __ ]		   	::(bracketl)ncbeforebody  	code 155;
 .			:: dotname 			code 156;
 __ __ ]		::(bracketl,dotname)ncbeforebody  code 155; 

 __	::(rule)ncrule; 
 __	::(ncrule)ncrulelist;
 __ __  ::(ncrulelist,ncrule)ncrulelist;


 __  __ ::(beforebody,afterrightside)rule		code 184;
                       {{ term will be parsed by another parser !!!!!!}}
 __  __ ::(ncbeforebody,afterrightside)ncrule		code 184;
                       {{ term will be parsed by another parser !!!!!!}}

 rules		::RWrules				code 150;

 __             ::(endofrules)afterrightside;
 __ __          ::(beforewhere,afterrightside)afterrightside;

 __             ::(endofrules1)afterrightside1;
 __ __          ::(beforewhere,afterrightside1)afterrightside1;


 choose		:: beforewhere				code 161;

 if             ::beforewhere                           code 139;
 __ := ()       ::(beforewhere2)beforewhere             code 138;
 __ := (__)     ::(beforewhere2,stratbody)beforewhere	code 142;  
 where __       ::(identifier)beforewhere2              code 137;

{{PATTERN}}
 __		:: (imodule) pattype			code 146;
 where (__)     :: (pattype)beforewhere      		code 147;

 end            ::endofstrategies;
 end            ::endofrules                            code 136;
 end            ::endofrules1                           code 151;

 __             ::(declare)declarelist;
 __ __          ::(declarelist,declare)declarelist;

 __ :  __ ;     ::(varlist,vartype) declare             code 135;
    :  __ ;	::(vartype) declare			code 135;

 __     ::(varname)varlist;
 __,__  ::(varlist, varname)varlist;
 '__'    ::(varname)varlist;
 __,'__'  ::(varlist, varname)varlist;
 
 __	::( identifier ) varname			code 133;
 $__	::( identifier ) varname			code 133;

{{ of strategy }}
 __		::(strategydesc) stratlist;
 __ __		::(stratlist,strategydesc) stratlist;

 repeat+ 	::repbegin				code 163;
 repeat*	::whilebegin				code 163;
 iterate* 	::iterbegin				code 163;
 iterate+ 	::iterplusbegin				code 163;

 tall		::tallbegin				code 163;
 tone		::tonebegin				code 163;
 tsome		::tsomebegin				code 163;
 rewrite	::rewritebegin				code 163;

 normalise	::normbegin				code 163;
 normalize	::normbegin				code 163;
 __ ( __ )	:: (normbegin, stratbody) strategy	code 170;
 __ ( __ )	:: (normbegin, strtypedlist) strategy	code 214;

 normalise	:: sssymbol				code 26;
 normalize	:: sssymbol				code 26;

 __ ( __ )	:: (repbegin, stratbody) strategy	code 171;
 __ ( __ )	:: (whilebegin, stratbody) strategy	code 164;
 __ ( __ )	:: (iterbegin,stratbody) strategy	code 165;
 __ ( __ )	:: (iterplusbegin,stratbody) strategy	code 166;

 __ ( __ )	:: (tallbegin,stratbody) strategy	code 252;
 __ ( __ )	:: (tonebegin,stratbody) strategy	code 253;
 __ ( __ )	:: (tsomebegin,stratbody) strategy	code 254;
 __ ( __ )	:: (rewritebegin,stratbody) strategy	code 255;

 dccall(__,__,__)	:: (prname, numofcall, restype)strategy	code 191;
 dkcall(__,__,__)	:: (prname, numofcall, restype)strategy code 192;

 META			:: strategy			code 195; {{ builtin(2)}}

 id			:: strategy				code 196;
 fail			:: strategy				code 190;
 #inline#		:: strategy 				code 203;

 __ __ )		:: (first2beg, strlist) strategy	code 174;
 __ __ )		:: (dontcare2beg, strlist) strategy	code 175;
 __ __ )		:: (normin, strlist) strategy       	code 250;
 __ __ )		:: (normout, strlist) strategy       	code 251;
 __ __ )		:: (dontcarecon2beg, strlist) strategy	code 162;
 __ __ )		:: (dontknow2beg, strlist) strategy	code 176;
 __ __ )		:: (dontknowcon2beg, strlist) strategy	code 181;
 __ __ )		:: (one2beg, strlist) strategy		code 179;
 __ __ )		:: (onecon2beg, strlist) strategy	code 168;


 __		::(stratcall) stratbody			code 159; 
 __		::(strategy) stratbody			code 160;
 __ ; __	::(stratbody, stratcall) stratbody	code 159; 
 __ ; __	::(stratbody, strategy) stratbody	code 160; 

 __		::(identifier) stratname		code 158;
 __		::(identifier) stratcall		code 180;
 __		::(identifier)prname			code 193;
 __		::(number)numofcall			code 194;
 __		::(imodule)restype			code 197;

 first choose 	  (	::first2beg			code 163;
 dont care choose (	::dontcare2beg			code 163;
 dont know choose (	::dontknow2beg			code 163;
 dont care one (	::one2beg			code 163;
 first (		::first2beg			code 163;
 dk (			::dontknow2beg			code 163;
 normin (              ::normin                       code 163;
 normout (             ::normout                      code 163;
 dc (                   ::dontcare2beg                  code 163;

 oneconcur (		::onecon2beg			code 163;
 dkconcur (		::dontknowcon2beg		code 163;
 dcconcur (		::dontcarecon2beg		code 163;

{{ TO BE DELETED  one (			::one2beg			code 163;}}
 dc one (		::one2beg			code 163;
 first one (		::one2beg			code 163;
 dcOne (		::one2beg			code 163;
 firstOne (		::one2beg			code 163;

 __		::(stratbody) strlist			code 177;
{{ ****
 WILL BE USED FOR concurrent strats --
 __ || __	::(strlist, stratbody) strlist		code 178; *** }}
 __ , __	::(strlist, stratbody) strlist		code 178; 

{{ pour Normalise }}
 __ : __	::(stratbody, sntype) strtypedlist			code 211;
{{ ****
 WILL BE USED FOR concurrent strats --
 __ || __ : __	::(strtypedlist, stratbody, sntype) strtypedlist	code 212; ***}}
 __ , __ : __	::(strtypedlist, stratbody, sntype) strtypedlist	code 212; 
 __             ::(imodule) sntype                     			code 213;

 #inline# 	:: inlinelist				code 205;


 :		:: opbegin				code 329;

 __ __ __ __ 	::(ssymbollist,opbegin,profil,atribl) opdef1;
 __ __ __        ::(ssymbollist,opbegin,profil) opdef1;

 __ __ __  __ 	::(RWglobal,strdeflist,RWlocal,strdeflist)strs;
 __ __ __  __ 	::(RWlocal,strdeflist,RWglobal,strdeflist)strs;
 __ __ 	      	::(RWglobal,strdeflist)strs;
 __ __ 	      	::(RWlocal,strdeflist)strs;

 __ 		::(strdef)strdeflist;
 __ __  	::(strdeflist,strdef)strdeflist;

 __ __ __  __    ::(ssymbollist,opbegin,profil,atribl) strdef1 code 334;
 __ __ __        ::(ssymbollist,opbegin,profil) strdef1	code 334;
 __ ;	     	::(strdef1)strdef				code 22; 

 ->		:: parrow					code 324;


 {{--- DEFINED --- }}
 __ __          :: (rest2, afterrightside1) sdeclars;
 __ __ __	:: (sdeclars, rest2, afterrightside1) sdeclars;

 {{--- EXPLICIT ---}}
  __ 		:: (ncrulelist) sdeclarsex;

 {{--- IMPLICIT ---}}

 __ __ ]	::(bracketl,trname)ncbeforebody1   code 333;
 __ ]		::(bracketl)ncbeforebody1	   code 333;
 __ __ ]	::(bracketl,dotname)ncbeforebody1  code 333;


 __ __ => __ __ :: (ncbeforebody1,stratname,stratbody,endofrules) 
			sdeclarsim1	code 167; {{ built-in strategy }}
 __ __ 		:: (ncbeforebody1,afterrightside1) sdeclarsim1; 
		{{defined strategy}}

 __ 		:: (sdeclarsim1) sdeclarsim;
 __  __ 	:: (sdeclarsim1,sdeclarsim) sdeclarsim;


 __             :: (bracketl) rest2                    code 340;

 __		::(trname1) trname;
 __(__) 	::(trname1,varlist1) trname;
 __             ::(identifier)trname1                  code 154;

 __     	::(varname1)varlist1;
 __,__  	::(varlist1, varname1)varlist1;
 
 __   		::(identifier) varname1                code 351;

 __ := [       ::(beforewhere2)beforewhere             code 353;


{{ __ 		:: (imodule) Gtype			code 361;}}
{{!!! 060698 !!!}}

 __		::(mimodule)Gtype			code 361;
 __[__] 	::(mimodule,Actarglist)Gtypeaux		code 186;
 __ 		:: (Gtypeaux) Gtype			code 361;

{{!!! 060698 !!!}}

 < __ >		:: (Gtype) Gtype			code 362;
 < __ -> __ >	:: (Gtype, Gtype) Gtype			code 363;
 __		:: (Gtype) vartype			code 364;

 __ 		:: (Gtype) stype			code 365;
 __ -> __	:: (Gtype, Gtype) stype			code 366;

 __ 		:: (Gtype) rtype			code 367;
 __ -> __	:: (Gtype, Gtype) rtype			code 368;


::
axiom
for module: 

end
