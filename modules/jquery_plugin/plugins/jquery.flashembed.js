/** 
 * flashembed 0.34. Adobe Flash embedding script
 * 
 * http://flowplayer.org/tools/flash-embed.html
 *
 * Copyright (c) 2008 Tero Piirainen (support@flowplayer.org)
 *
 * Dual licensed under MIT and GPL 2+ licenses
 * SEE: http://www.opensource.org/licenses
 *
 * first version 0.01 - 03/11/2008 
 * version 0.34 - Fri Nov 28 2008 09:04:32 GMT-0000 (GMT+00:00)
 */
(function(){var jQ=typeof jQuery=='function';function isDomReady(){if(domReady.done){return false;}var d=document;if(d&&d.getElementsByTagName&&d.getElementById&&d.body){clearInterval(domReady.timer);domReady.timer=null;for(var i=0;i<domReady.ready.length;i++){domReady.ready[i].call();}domReady.ready=null;domReady.done=true;}}var domReady=jQ?jQuery:function(f){if(domReady.done){return f();}if(domReady.timer){domReady.ready.push(f);}else{domReady.ready=[f];domReady.timer=setInterval(isDomReady,13);}};function extend(to,from){if(from){for(key in from){if(from.hasOwnProperty(key)){to[key]=from[key];}}}return to;}function concatVars(vars){var out="";for(var key in vars){if(vars[key]){out+=[key]+'='+asString(vars[key])+'&';}}return out.substring(0,out.length-1);}function asString(obj){switch(typeOf(obj)){case'string':obj=obj.replace(new RegExp('(["\\\\])','g'),'\\$1');obj=obj.replace(/^\s?(\d+)%/,"$1pct");return'"'+obj+'"';case'array':return'['+map(obj,function(el){return asString(el);}).join(',')+']';case'function':return'"function()"';case'object':var str=[];for(var prop in obj){if(obj.hasOwnProperty(prop)){str.push('"'+prop+'":'+asString(obj[prop]));}}return'{'+str.join(',')+'}';}return String(obj).replace(/\s/g," ").replace(/\'/g,"\"");}function typeOf(obj){if(obj===null||obj===undefined){return false;}var type=typeof obj;return(type=='object'&&obj.push)?'array':type;}if(window.attachEvent){window.attachEvent("onbeforeunload",function(){__flash_unloadHandler=function(){};__flash_savedUnloadHandler=function(){};});}function map(arr,func){var newArr=[];for(var i in arr){if(arr.hasOwnProperty(i)){newArr[i]=func(arr[i]);}}return newArr;}function getEmbedCode(p,c){var html='<embed type="application/x-shockwave-flash" ';if(p.id){extend(p,{name:p.id});}for(var key in p){if(p[key]!==null){html+=key+'="'+p[key]+'"\n\t';}}if(c){html+='flashvars=\''+concatVars(c)+'\'';}html+='/>';return html;}function getObjectCode(p,c,embeddable){var html='<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" ';html+='width="'+p.width+'" height="'+p.height+'"';if(!p.id&&document.all){p.id="_"+(""+Math.random()).substring(5);}if(p.id){html+=' id="'+p.id+'"';}html+='>';if(document.all){p.src+=((p.src.indexOf("?")!=-1?"&":"?")+Math.random());}html+='\n\t<param name="movie" value="'+p.src+'" />';var e=extend({},p);e.id=e.width=e.height=e.src=null;for(var k in e){if(e[k]!==null){html+='\n\t<param name="'+k+'" value="'+e[k]+'" />';}}if(c){html+='\n\t<param name="flashvars" value=\''+concatVars(c)+'\' />';}if(embeddable){html+=getEmbedCode(p,c);}html+="</object>";return html;}function getFullHTML(p,c){return getObjectCode(p,c,true);}function getHTML(p,c){var isNav=navigator.plugins&&navigator.mimeTypes&&navigator.mimeTypes.length;return(isNav)?getEmbedCode(p,c):getObjectCode(p,c);}window.flashembed=function(root,userParams,flashvars){var params={src:'#',width:'100%',height:'100%',version:null,onFail:null,expressInstall:null,debug:false,allowfullscreen:true,allowscriptaccess:'always',quality:'high',type:'application/x-shockwave-flash',pluginspage:'http://www.adobe.com/go/getflashplayer'};if(typeof userParams=='string'){userParams={src:userParams};}extend(params,userParams);var version=flashembed.getVersion();var required=params.version;var express=params.expressInstall;var debug=params.debug;if(typeof root=='string'){var el=document.getElementById(root);if(el){root=el;}else{domReady(function(){flashembed(root,userParams,flashvars);});return;}}if(!root){return;}if(!required||flashembed.isSupported(required)){params.onFail=params.version=params.expressInstall=params.debug=null;root.innerHTML=getHTML(params,flashvars);return root.firstChild;}else if(params.onFail){var ret=params.onFail.call(params,flashembed.getVersion(),flashvars);if(ret===true){root.innerHTML=ret;}}else if(required&&express&&flashembed.isSupported([6,65])){extend(params,{src:express});flashvars={MMredirectURL:location.href,MMplayerType:'PlugIn',MMdoctitle:document.title};root.innerHTML=getHTML(params,flashvars);}else{if(root.innerHTML.replace(/\s/g,'')!==''){}else{root.innerHTML="<h2>Flash version "+required+" or greater is required</h2>"+"<h3>"+(version[0]>0?"Your version is "+version:"You have no flash plugin installed")+"</h3>"+"<p>Download latest version from <a href='"+params.pluginspage+"'>here</a></p>";}}return root;};extend(window.flashembed,{getVersion:function(){var version=[0,0];if(navigator.plugins&&typeof navigator.plugins["Shockwave Flash"]=="object"){var _d=navigator.plugins["Shockwave Flash"].description;if(typeof _d!="undefined"){_d=_d.replace(/^.*\s+(\S+\s+\S+$)/,"$1");var _m=parseInt(_d.replace(/^(.*)\..*$/,"$1"),10);var _r=/r/.test(_d)?parseInt(_d.replace(/^.*r(.*)$/,"$1"),10):0;version=[_m,_r];}}else if(window.ActiveXObject){try{var _a=new ActiveXObject("ShockwaveFlash.ShockwaveFlash.7");}catch(e){try{_a=new ActiveXObject("ShockwaveFlash.ShockwaveFlash.6");version=[6,0];_a.AllowScriptAccess="always";}catch(ee){if(version[0]==6){return;}}try{_a=new ActiveXObject("ShockwaveFlash.ShockwaveFlash");}catch(eee){}}if(typeof _a=="object"){_d=_a.GetVariable("$version");if(typeof _d!="undefined"){_d=_d.replace(/^\S+\s+(.*)$/,"$1").split(",");version=[parseInt(_d[0],10),parseInt(_d[2],10)];}}}return version;},isSupported:function(version){var now=flashembed.getVersion();var ret=(now[0]>version[0])||(now[0]==version[0]&&now[1]>=version[1]);return ret;},domReady:domReady,asString:asString,getHTML:getHTML,getFullHTML:getFullHTML});if(jQ){jQuery.prototype.flashembed=function(params,flashvars){return this.each(function(){flashembed(this,params,flashvars);});};}})();