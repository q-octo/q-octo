(function(){const _=document.createElement("link").relList;if(_&&_.supports&&_.supports("modulepreload"))return;for(const o of document.querySelectorAll('link[rel="modulepreload"]'))n(o);new MutationObserver(o=>{for(const r of o)if(r.type==="childList")for(const l of r.addedNodes)l.tagName==="LINK"&&l.rel==="modulepreload"&&n(l)}).observe(document,{childList:!0,subtree:!0});function t(o){const r={};return o.integrity&&(r.integrity=o.integrity),o.referrerPolicy&&(r.referrerPolicy=o.referrerPolicy),o.crossOrigin==="use-credentials"?r.credentials="include":o.crossOrigin==="anonymous"?r.credentials="omit":r.credentials="same-origin",r}function n(o){if(o.ep)return;o.ep=!0;const r=t(o);fetch(o.href,r)}})();var V,h,ve,C,ne,me,z,Y,G,J,D={},ye=[],Ce=/acit|ex(?:s|g|n|p|$)|rph|grid|ows|mnc|ntw|ine[ch]|zoo|^ord|itera/i,Z=Array.isArray;function $(e,_){for(var t in _)e[t]=_[t];return e}function ge(e){var _=e.parentNode;_&&_.removeChild(e)}function He(e,_,t){var n,o,r,l={};for(r in _)r=="key"?n=_[r]:r=="ref"?o=_[r]:l[r]=_[r];if(arguments.length>2&&(l.children=arguments.length>3?V.call(arguments,2):t),typeof e=="function"&&e.defaultProps!=null)for(r in e.defaultProps)l[r]===void 0&&(l[r]=e.defaultProps[r]);return M(e,l,n,o,null)}function M(e,_,t,n,o){var r={type:e,props:_,key:t,ref:n,__k:null,__:null,__b:0,__e:null,__d:void 0,__c:null,constructor:void 0,__v:o??++ve,__i:-1,__u:0};return o==null&&h.vnode!=null&&h.vnode(r),r}function B(e){return e.children}function U(e,_){this.props=e,this.context=_}function H(e,_){if(_==null)return e.__?H(e.__,e.__i+1):null;for(var t;_<e.__k.length;_++)if((t=e.__k[_])!=null&&t.__e!=null)return t.__e;return typeof e.type=="function"?H(e):null}function be(e){var _,t;if((e=e.__)!=null&&e.__c!=null){for(e.__e=e.__c.base=null,_=0;_<e.__k.length;_++)if((t=e.__k[_])!=null&&t.__e!=null){e.__e=e.__c.base=t.__e;break}return be(e)}}function oe(e){(!e.__d&&(e.__d=!0)&&C.push(e)&&!R.__r++||ne!==h.debounceRendering)&&((ne=h.debounceRendering)||me)(R)}function R(){var e,_,t,n,o,r,l,u;for(C.sort(z);e=C.shift();)e.__d&&(_=C.length,n=void 0,r=(o=(t=e).__v).__e,l=[],u=[],t.__P&&((n=$({},o)).__v=o.__v+1,h.vnode&&h.vnode(n),ee(t.__P,n,o,t.__n,t.__P.ownerSVGElement!==void 0,32&o.__u?[r]:null,l,r??H(o),!!(32&o.__u),u),n.__v=o.__v,n.__.__k[n.__i]=n,Se(l,n,u),n.__e!=r&&be(n)),C.length>_&&C.sort(z));R.__r=0}function ke(e,_,t,n,o,r,l,u,s,c,a){var i,d,f,g,w,b=n&&n.__k||ye,v=_.length;for(t.__d=s,Ne(t,_,b),s=t.__d,i=0;i<v;i++)(f=t.__k[i])!=null&&typeof f!="boolean"&&typeof f!="function"&&(d=f.__i===-1?D:b[f.__i]||D,f.__i=i,ee(e,f,d,o,r,l,u,s,c,a),g=f.__e,f.ref&&d.ref!=f.ref&&(d.ref&&_e(d.ref,null,f),a.push(f.ref,f.__c||g,f)),w==null&&g!=null&&(w=g),65536&f.__u||d.__k===f.__k?(s&&!s.isConnected&&(s=H(d)),s=xe(f,s,e)):typeof f.type=="function"&&f.__d!==void 0?s=f.__d:g&&(s=g.nextSibling),f.__d=void 0,f.__u&=-196609);t.__d=s,t.__e=w}function Ne(e,_,t){var n,o,r,l,u,s=_.length,c=t.length,a=c,i=0;for(e.__k=[],n=0;n<s;n++)l=n+i,(o=e.__k[n]=(o=_[n])==null||typeof o=="boolean"||typeof o=="function"?null:typeof o=="string"||typeof o=="number"||typeof o=="bigint"||o.constructor==String?M(null,o,null,null,null):Z(o)?M(B,{children:o},null,null,null):o.constructor===void 0&&o.__b>0?M(o.type,o.props,o.key,o.ref?o.ref:null,o.__v):o)!=null?(o.__=e,o.__b=e.__b+1,u=Ee(o,t,l,a),o.__i=u,r=null,u!==-1&&(a--,(r=t[u])&&(r.__u|=131072)),r==null||r.__v===null?(u==-1&&i--,typeof o.type!="function"&&(o.__u|=65536)):u!==l&&(u===l+1?i++:u>l?a>s-l?i+=u-l:i--:u<l?u==l-1&&(i=u-l):i=0,u!==n+i&&(o.__u|=65536))):(r=t[l])&&r.key==null&&r.__e&&!(131072&r.__u)&&(r.__e==e.__d&&(e.__d=H(r)),K(r,r,!1),t[l]=null,a--);if(a)for(n=0;n<c;n++)(r=t[n])!=null&&!(131072&r.__u)&&(r.__e==e.__d&&(e.__d=H(r)),K(r,r))}function xe(e,_,t){var n,o;if(typeof e.type=="function"){for(n=e.__k,o=0;n&&o<n.length;o++)n[o]&&(n[o].__=e,_=xe(n[o],_,t));return _}e.__e!=_&&(t.insertBefore(e.__e,_||null),_=e.__e);do _=_&&_.nextSibling;while(_!=null&&_.nodeType===8);return _}function Ee(e,_,t,n){var o=e.key,r=e.type,l=t-1,u=t+1,s=_[t];if(s===null||s&&o==s.key&&r===s.type&&!(131072&s.__u))return t;if(n>(s!=null&&!(131072&s.__u)?1:0))for(;l>=0||u<_.length;){if(l>=0){if((s=_[l])&&!(131072&s.__u)&&o==s.key&&r===s.type)return l;l--}if(u<_.length){if((s=_[u])&&!(131072&s.__u)&&o==s.key&&r===s.type)return u;u++}}return-1}function re(e,_,t){_[0]==="-"?e.setProperty(_,t??""):e[_]=t==null?"":typeof t!="number"||Ce.test(_)?t:t+"px"}function L(e,_,t,n,o){var r;e:if(_==="style")if(typeof t=="string")e.style.cssText=t;else{if(typeof n=="string"&&(e.style.cssText=n=""),n)for(_ in n)t&&_ in t||re(e.style,_,"");if(t)for(_ in t)n&&t[_]===n[_]||re(e.style,_,t[_])}else if(_[0]==="o"&&_[1]==="n")r=_!==(_=_.replace(/(PointerCapture)$|Capture$/i,"$1")),_=_.toLowerCase()in e||_==="onFocusOut"||_==="onFocusIn"?_.toLowerCase().slice(2):_.slice(2),e.l||(e.l={}),e.l[_+r]=t,t?n?t.u=n.u:(t.u=Y,e.addEventListener(_,r?J:G,r)):e.removeEventListener(_,r?J:G,r);else{if(o)_=_.replace(/xlink(H|:h)/,"h").replace(/sName$/,"s");else if(_!="width"&&_!="height"&&_!="href"&&_!="list"&&_!="form"&&_!="tabIndex"&&_!="download"&&_!="rowSpan"&&_!="colSpan"&&_!="role"&&_ in e)try{e[_]=t??"";break e}catch{}typeof t=="function"||(t==null||t===!1&&_[4]!=="-"?e.removeAttribute(_):e.setAttribute(_,t))}}function ie(e){return function(_){if(this.l){var t=this.l[_.type+e];if(_.t==null)_.t=Y++;else if(_.t<t.u)return;return t(h.event?h.event(_):_)}}}function ee(e,_,t,n,o,r,l,u,s,c){var a,i,d,f,g,w,b,v,k,P,T,E,te,A,q,S=_.type;if(_.constructor!==void 0)return null;128&t.__u&&(s=!!(32&t.__u),r=[u=_.__e=t.__e]),(a=h.__b)&&a(_);e:if(typeof S=="function")try{if(v=_.props,k=(a=S.contextType)&&n[a.__c],P=a?k?k.props.value:a.__:n,t.__c?b=(i=_.__c=t.__c).__=i.__E:("prototype"in S&&S.prototype.render?_.__c=i=new S(v,P):(_.__c=i=new U(v,P),i.constructor=S,i.render=Te),k&&k.sub(i),i.props=v,i.state||(i.state={}),i.context=P,i.__n=n,d=i.__d=!0,i.__h=[],i._sb=[]),i.__s==null&&(i.__s=i.state),S.getDerivedStateFromProps!=null&&(i.__s==i.state&&(i.__s=$({},i.__s)),$(i.__s,S.getDerivedStateFromProps(v,i.__s))),f=i.props,g=i.state,i.__v=_,d)S.getDerivedStateFromProps==null&&i.componentWillMount!=null&&i.componentWillMount(),i.componentDidMount!=null&&i.__h.push(i.componentDidMount);else{if(S.getDerivedStateFromProps==null&&v!==f&&i.componentWillReceiveProps!=null&&i.componentWillReceiveProps(v,P),!i.__e&&(i.shouldComponentUpdate!=null&&i.shouldComponentUpdate(v,i.__s,P)===!1||_.__v===t.__v)){for(_.__v!==t.__v&&(i.props=v,i.state=i.__s,i.__d=!1),_.__e=t.__e,_.__k=t.__k,_.__k.forEach(function(F){F&&(F.__=_)}),T=0;T<i._sb.length;T++)i.__h.push(i._sb[T]);i._sb=[],i.__h.length&&l.push(i);break e}i.componentWillUpdate!=null&&i.componentWillUpdate(v,i.__s,P),i.componentDidUpdate!=null&&i.__h.push(function(){i.componentDidUpdate(f,g,w)})}if(i.context=P,i.props=v,i.__P=e,i.__e=!1,E=h.__r,te=0,"prototype"in S&&S.prototype.render){for(i.state=i.__s,i.__d=!1,E&&E(_),a=i.render(i.props,i.state,i.context),A=0;A<i._sb.length;A++)i.__h.push(i._sb[A]);i._sb=[]}else do i.__d=!1,E&&E(_),a=i.render(i.props,i.state,i.context),i.state=i.__s;while(i.__d&&++te<25);i.state=i.__s,i.getChildContext!=null&&(n=$($({},n),i.getChildContext())),d||i.getSnapshotBeforeUpdate==null||(w=i.getSnapshotBeforeUpdate(f,g)),ke(e,Z(q=a!=null&&a.type===B&&a.key==null?a.props.children:a)?q:[q],_,t,n,o,r,l,u,s,c),i.base=_.__e,_.__u&=-161,i.__h.length&&l.push(i),b&&(i.__E=i.__=null)}catch(F){_.__v=null,s||r!=null?(_.__e=u,_.__u|=s?160:32,r[r.indexOf(u)]=null):(_.__e=t.__e,_.__k=t.__k),h.__e(F,_,t)}else r==null&&_.__v===t.__v?(_.__k=t.__k,_.__e=t.__e):_.__e=De(t.__e,_,t,n,o,r,l,s,c);(a=h.diffed)&&a(_)}function Se(e,_,t){_.__d=void 0;for(var n=0;n<t.length;n++)_e(t[n],t[++n],t[++n]);h.__c&&h.__c(_,e),e.some(function(o){try{e=o.__h,o.__h=[],e.some(function(r){r.call(o)})}catch(r){h.__e(r,o.__v)}})}function De(e,_,t,n,o,r,l,u,s){var c,a,i,d,f,g,w,b=t.props,v=_.props,k=_.type;if(k==="svg"&&(o=!0),r!=null){for(c=0;c<r.length;c++)if((f=r[c])&&"setAttribute"in f==!!k&&(k?f.localName===k:f.nodeType===3)){e=f,r[c]=null;break}}if(e==null){if(k===null)return document.createTextNode(v);e=o?document.createElementNS("http://www.w3.org/2000/svg",k):document.createElement(k,v.is&&v),r=null,u=!1}if(k===null)b===v||u&&e.data===v||(e.data=v);else{if(r=r&&V.call(e.childNodes),b=t.props||D,!u&&r!=null)for(b={},c=0;c<e.attributes.length;c++)b[(f=e.attributes[c]).name]=f.value;for(c in b)f=b[c],c=="children"||(c=="dangerouslySetInnerHTML"?i=f:c==="key"||c in v||L(e,c,null,f,o));for(c in v)f=v[c],c=="children"?d=f:c=="dangerouslySetInnerHTML"?a=f:c=="value"?g=f:c=="checked"?w=f:c==="key"||u&&typeof f!="function"||b[c]===f||L(e,c,f,b[c],o);if(a)u||i&&(a.__html===i.__html||a.__html===e.innerHTML)||(e.innerHTML=a.__html),_.__k=[];else if(i&&(e.innerHTML=""),ke(e,Z(d)?d:[d],_,t,n,o&&k!=="foreignObject",r,l,r?r[0]:t.__k&&H(t,0),u,s),r!=null)for(c=r.length;c--;)r[c]!=null&&ge(r[c]);u||(c="value",g!==void 0&&(g!==e[c]||k==="progress"&&!g||k==="option"&&g!==b[c])&&L(e,c,g,b[c],!1),c="checked",w!==void 0&&w!==e[c]&&L(e,c,w,b[c],!1))}return e}function _e(e,_,t){try{typeof e=="function"?e(_):e.current=_}catch(n){h.__e(n,t)}}function K(e,_,t){var n,o;if(h.unmount&&h.unmount(e),(n=e.ref)&&(n.current&&n.current!==e.__e||_e(n,null,_)),(n=e.__c)!=null){if(n.componentWillUnmount)try{n.componentWillUnmount()}catch(r){h.__e(r,_)}n.base=n.__P=null}if(n=e.__k)for(o=0;o<n.length;o++)n[o]&&K(n[o],_,t||typeof e.type!="function");t||e.__e==null||ge(e.__e),e.__c=e.__=e.__e=e.__d=void 0}function Te(e,_,t){return this.constructor(e,t)}function Ae(e,_,t){var n,o,r,l;h.__&&h.__(e,_),o=(n=typeof t=="function")?null:t&&t.__k||_.__k,r=[],l=[],ee(_,e=(!n&&t||_).__k=He(B,null,[e]),o||D,D,_.ownerSVGElement!==void 0,!n&&t?[t]:o?null:_.firstChild?V.call(_.childNodes):null,r,!n&&t?t:o?o.__e:_.firstChild,n,l),Se(r,e,l)}V=ye.slice,h={__e:function(e,_,t,n){for(var o,r,l;_=_.__;)if((o=_.__c)&&!o.__)try{if((r=o.constructor)&&r.getDerivedStateFromError!=null&&(o.setState(r.getDerivedStateFromError(e)),l=o.__d),o.componentDidCatch!=null&&(o.componentDidCatch(e,n||{}),l=o.__d),l)return o.__E=o}catch(u){e=u}throw e}},ve=0,U.prototype.setState=function(e,_){var t;t=this.__s!=null&&this.__s!==this.state?this.__s:this.__s=$({},this.state),typeof e=="function"&&(e=e($({},t),this.props)),e&&$(t,e),e!=null&&this.__v&&(_&&this._sb.push(_),oe(this))},U.prototype.forceUpdate=function(e){this.__v&&(this.__e=!0,e&&this.__h.push(e),oe(this))},U.prototype.render=B,C=[],me=typeof Promise=="function"?Promise.prototype.then.bind(Promise.resolve()):setTimeout,z=function(e,_){return e.__v.__b-_.__v.__b},R.__r=0,Y=0,G=ie(!1),J=ie(!0);var Fe=0;function p(e,_,t,n,o,r){var l,u,s={};for(u in _)u=="ref"?l=_[u]:s[u]=_[u];var c={type:e,props:s,key:t,ref:l,__k:null,__:null,__b:0,__e:null,__d:void 0,__c:null,constructor:void 0,__v:--Fe,__i:-1,__u:0,__source:o,__self:r};if(typeof e=="function"&&(l=e.defaultProps))for(u in l)s[u]===void 0&&(s[u]=l[u]);return h.vnode&&h.vnode(c),c}function x({...e}){return p("div",{className:"variable",style:{padding:"4px",display:"flex",flexDirection:"row",gap:"4px"},children:[e.title&&p("div",{children:p("strong",{children:[e.title,": "]})}),p("div",{children:p("em",{children:[e.value," ",e.unit]})})]})}function le({...e}){return p("div",{style:{border:"1px solid black",padding:"10px 32px",borderRadius:"5px"},children:[p("h5",{children:e.title}),p("div",{style:{display:"flex",flexDirection:"row",gap:"8px"},children:p(x,{title:"Temp",value:e.temperature,unit:"°C"})}),p("div",{style:{display:"flex",flexDirection:"row",gap:"8px"},children:[p(x,{title:"RPM",value:e.rpm,unit:"RPM"}),p(x,{title:"Angle",value:e.angle,unit:"°"})]})]})}function N({children:e,..._}){return p("div",{style:{display:"flex",padding:"12px",flexDirection:"row",gap:"12px",width:"100%",justifyContent:"center",alignItems:"center"},children:e})}function Le({children:e,..._}){return p("div",{style:{display:"flex",padding:"12px",flexDirection:"column",gap:"16px",justifyContent:"center",alignItems:"center",border:"1px solid black"},children:e})}var W,m,j,ue,X=0,we=[],O=[],y=h,ce=y.__b,se=y.__r,fe=y.diffed,ae=y.__c,pe=y.unmount,de=y.__;function Pe(e,_){y.__h&&y.__h(m,e,X||_),X=0;var t=m.__H||(m.__H={__:[],__h:[]});return e>=t.__.length&&t.__.push({__V:O}),t.__[e]}function Me(e){return X=1,Ue($e,e)}function Ue(e,_,t){var n=Pe(W++,2);if(n.t=e,!n.__c&&(n.__=[t?t(_):$e(void 0,_),function(u){var s=n.__N?n.__N[0]:n.__[0],c=n.t(s,u);s!==c&&(n.__N=[c,n.__[1]],n.__c.setState({}))}],n.__c=m,!m.u)){var o=function(u,s,c){if(!n.__c.__H)return!0;var a=n.__c.__H.__.filter(function(d){return!!d.__c});if(a.every(function(d){return!d.__N}))return!r||r.call(this,u,s,c);var i=!1;return a.forEach(function(d){if(d.__N){var f=d.__[0];d.__=d.__N,d.__N=void 0,f!==d.__[0]&&(i=!0)}}),!(!i&&n.__c.props===u)&&(!r||r.call(this,u,s,c))};m.u=!0;var r=m.shouldComponentUpdate,l=m.componentWillUpdate;m.componentWillUpdate=function(u,s,c){if(this.__e){var a=r;r=void 0,o(u,s,c),r=a}l&&l.call(this,u,s,c)},m.shouldComponentUpdate=o}return n.__N||n.__}function Oe(e,_){var t=Pe(W++,3);!y.__s&&We(t.__H,_)&&(t.__=e,t.i=_,m.__H.__h.push(t))}function Ie(){for(var e;e=we.shift();)if(e.__P&&e.__H)try{e.__H.__h.forEach(I),e.__H.__h.forEach(Q),e.__H.__h=[]}catch(_){e.__H.__h=[],y.__e(_,e.__v)}}y.__b=function(e){m=null,ce&&ce(e)},y.__=function(e,_){e&&_.__k&&_.__k.__m&&(e.__m=_.__k.__m),de&&de(e,_)},y.__r=function(e){se&&se(e),W=0;var _=(m=e.__c).__H;_&&(j===m?(_.__h=[],m.__h=[],_.__.forEach(function(t){t.__N&&(t.__=t.__N),t.__V=O,t.__N=t.i=void 0})):(_.__h.forEach(I),_.__h.forEach(Q),_.__h=[],W=0)),j=m},y.diffed=function(e){fe&&fe(e);var _=e.__c;_&&_.__H&&(_.__H.__h.length&&(we.push(_)!==1&&ue===y.requestAnimationFrame||((ue=y.requestAnimationFrame)||Re)(Ie)),_.__H.__.forEach(function(t){t.i&&(t.__H=t.i),t.__V!==O&&(t.__=t.__V),t.i=void 0,t.__V=O})),j=m=null},y.__c=function(e,_){_.some(function(t){try{t.__h.forEach(I),t.__h=t.__h.filter(function(n){return!n.__||Q(n)})}catch(n){_.some(function(o){o.__h&&(o.__h=[])}),_=[],y.__e(n,t.__v)}}),ae&&ae(e,_)},y.unmount=function(e){pe&&pe(e);var _,t=e.__c;t&&t.__H&&(t.__H.__.forEach(function(n){try{I(n)}catch(o){_=o}}),t.__H=void 0,_&&y.__e(_,t.__v))};var he=typeof requestAnimationFrame=="function";function Re(e){var _,t=function(){clearTimeout(n),he&&cancelAnimationFrame(_),setTimeout(e)},n=setTimeout(t,100);he&&(_=requestAnimationFrame(t))}function I(e){var _=m,t=e.__c;typeof t=="function"&&(e.__c=void 0,t()),m=_}function Q(e){var _=m;e.__c=e.__(),m=_}function We(e,_){return!e||e.length!==_.length||_.some(function(t,n){return t!==e[n]})}function $e(e,_){return typeof _=="function"?_(e):_}function Ve(){const[e,_]=Me({motors:{motor1:{temperature:22,rpm:40,angle:3},motor2:{temperature:24,rpm:50,angle:3}},batteries:{number:4,status:"NOT_X"},controlSource:"RC",fuel:50,voltage:12,current:2,rssi:-50,signalStrength:-50,offset:{left:0,right:0}});return Oe(()=>{const t=new WebSocket("ws://"+location.host+"/echo");return t.onopen=()=>{console.log("Connected to server")},t.onmessage=n=>{const o=JSON.parse(n.data);_(o)},()=>{t.close()}},[]),p(Le,{children:[p(le,{title:"Motor 1",temperature:e.motors.motor1.temperature,rpm:e.motors.motor1.rpm,angle:e.motors.motor1.angle}),p(le,{title:"Motor 2",temperature:e.motors.motor2.temperature,rpm:e.motors.motor2.rpm,angle:e.motors.motor2.angle}),p(N,{children:[p(x,{title:"No. Bats",value:e.batteries.number}),p(x,{title:"Status",value:e.batteries.status})]}),p(N,{children:p(x,{title:"Control Source",value:e.controlSource})}),p(N,{children:p(x,{title:"Fuel",value:e.fuel,unit:"%"})}),p(x,{title:"Voltage",value:e.voltage,unit:"V"}),p(x,{title:"Current",value:e.current,unit:"A"}),p(N,{}),p(N,{children:[p(x,{title:"RSSI",value:e.rssi,unit:"dBm"}),p(x,{title:"Signal",value:e.signalStrength,unit:"dBm"})]}),p(N,{children:[p(x,{title:"Offset Left",value:e.offset.left,unit:"°"}),p(x,{title:"Offset Right",value:e.offset.right,unit:"°"})]})]})}Ae(p(Ve,{}),document.getElementById("app"));
