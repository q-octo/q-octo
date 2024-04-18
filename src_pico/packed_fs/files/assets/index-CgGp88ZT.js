(function(){const t=document.createElement("link").relList;if(t&&t.supports&&t.supports("modulepreload"))return;for(const r of document.querySelectorAll('link[rel="modulepreload"]'))_(r);new MutationObserver(r=>{for(const i of r)if(i.type==="childList")for(const s of i.addedNodes)s.tagName==="LINK"&&s.rel==="modulepreload"&&_(s)}).observe(document,{childList:!0,subtree:!0});function e(r){const i={};return r.integrity&&(i.integrity=r.integrity),r.referrerPolicy&&(i.referrerPolicy=r.referrerPolicy),r.crossOrigin==="use-credentials"?i.credentials="include":r.crossOrigin==="anonymous"?i.credentials="omit":i.credentials="same-origin",i}function _(r){if(r.ep)return;r.ep=!0;const i=e(r);fetch(r.href,i)}})();var Z,p,Et,S,pt,Pt,et,lt,nt,rt,B={},At=[],Dt=/acit|ex(?:s|g|n|p|$)|rph|grid|ows|mnc|ntw|ine[ch]|zoo|^ord|itera/i,at=Array.isArray;function U(n,t){for(var e in t)n[e]=t[e];return n}function Rt(n){var t=n.parentNode;t&&t.removeChild(n)}function Vt(n,t,e){var _,r,i,s={};for(i in t)i=="key"?_=t[i]:i=="ref"?r=t[i]:s[i]=t[i];if(arguments.length>2&&(s.children=arguments.length>3?Z.call(arguments,2):e),typeof n=="function"&&n.defaultProps!=null)for(i in n.defaultProps)s[i]===void 0&&(s[i]=n.defaultProps[i]);return D(n,s,_,r,null)}function D(n,t,e,_,r){var i={type:n,props:t,key:e,ref:_,__k:null,__:null,__b:0,__e:null,__d:void 0,__c:null,constructor:void 0,__v:r??++Et,__i:-1,__u:0};return r==null&&p.vnode!=null&&p.vnode(i),i}function J(n){return n.children}function V(n,t){this.props=n,this.context=t}function E(n,t){if(t==null)return n.__?E(n.__,n.__i+1):null;for(var e;t<n.__k.length;t++)if((e=n.__k[t])!=null&&e.__e!=null)return e.__e;return typeof n.type=="function"?E(n):null}function Ct(n){var t,e;if((n=n.__)!=null&&n.__c!=null){for(n.__e=n.__c.base=null,t=0;t<n.__k.length;t++)if((e=n.__k[t])!=null&&e.__e!=null){n.__e=n.__c.base=e.__e;break}return Ct(n)}}function bt(n){(!n.__d&&(n.__d=!0)&&S.push(n)&&!z.__r++||pt!==p.debounceRendering)&&((pt=p.debounceRendering)||Pt)(z)}function z(){var n,t,e,_,r,i,s,l;for(S.sort(et);n=S.shift();)n.__d&&(t=S.length,_=void 0,i=(r=(e=n).__v).__e,s=[],l=[],e.__P&&((_=U({},r)).__v=r.__v+1,p.vnode&&p.vnode(_),ct(e.__P,_,r,e.__n,e.__P.ownerSVGElement!==void 0,32&r.__u?[i]:null,s,i??E(r),!!(32&r.__u),l),_.__v=r.__v,_.__.__k[_.__i]=_,$t(s,_,l),_.__e!=i&&Ct(_)),S.length>t&&S.sort(et));z.__r=0}function Bt(n,t,e,_,r,i,s,l,c,a,h){var o,f,d,v,T,y=_&&_.__k||At,b=t.length;for(e.__d=c,Wt(e,t,y),c=e.__d,o=0;o<b;o++)(d=e.__k[o])!=null&&typeof d!="boolean"&&typeof d!="function"&&(f=d.__i===-1?B:y[d.__i]||B,d.__i=o,ct(n,d,f,r,i,s,l,c,a,h),v=d.__e,d.ref&&f.ref!=d.ref&&(f.ref&&dt(f.ref,null,d),h.push(d.ref,d.__c||v,d)),T==null&&v!=null&&(T=v),65536&d.__u||f.__k===d.__k?(c&&!c.isConnected&&(c=E(f)),c=Ot(d,c,n)):typeof d.type=="function"&&d.__d!==void 0?c=d.__d:v&&(c=v.nextSibling),d.__d=void 0,d.__u&=-196609);e.__d=c,e.__e=T}function Wt(n,t,e){var _,r,i,s,l,c=t.length,a=e.length,h=a,o=0;for(n.__k=[],_=0;_<c;_++)s=_+o,(r=n.__k[_]=(r=t[_])==null||typeof r=="boolean"||typeof r=="function"?null:typeof r=="string"||typeof r=="number"||typeof r=="bigint"||r.constructor==String?D(null,r,null,null,null):at(r)?D(J,{children:r},null,null,null):r.constructor===void 0&&r.__b>0?D(r.type,r.props,r.key,r.ref?r.ref:null,r.__v):r)!=null?(r.__=n,r.__b=n.__b+1,l=jt(r,e,s,h),r.__i=l,i=null,l!==-1&&(h--,(i=e[l])&&(i.__u|=131072)),i==null||i.__v===null?(l==-1&&o--,typeof r.type!="function"&&(r.__u|=65536)):l!==s&&(l===s+1?o++:l>s?h>c-s?o+=l-s:o--:l<s?l==s-1&&(o=l-s):o=0,l!==_+o&&(r.__u|=65536))):(i=e[s])&&i.key==null&&i.__e&&!(131072&i.__u)&&(i.__e==n.__d&&(n.__d=E(i)),_t(i,i,!1),e[s]=null,h--);if(h)for(_=0;_<a;_++)(i=e[_])!=null&&!(131072&i.__u)&&(i.__e==n.__d&&(n.__d=E(i)),_t(i,i))}function Ot(n,t,e){var _,r;if(typeof n.type=="function"){for(_=n.__k,r=0;_&&r<_.length;r++)_[r]&&(_[r].__=n,t=Ot(_[r],t,e));return t}n.__e!=t&&(e.insertBefore(n.__e,t||null),t=n.__e);do t=t&&t.nextSibling;while(t!=null&&t.nodeType===8);return t}function jt(n,t,e,_){var r=n.key,i=n.type,s=e-1,l=e+1,c=t[e];if(c===null||c&&r==c.key&&i===c.type&&!(131072&c.__u))return e;if(_>(c!=null&&!(131072&c.__u)?1:0))for(;s>=0||l<t.length;){if(s>=0){if((c=t[s])&&!(131072&c.__u)&&r==c.key&&i===c.type)return s;s--}if(l<t.length){if((c=t[l])&&!(131072&c.__u)&&r==c.key&&i===c.type)return l;l++}}return-1}function mt(n,t,e){t[0]==="-"?n.setProperty(t,e??""):n[t]=e==null?"":typeof e!="number"||Dt.test(t)?e:e+"px"}function M(n,t,e,_,r){var i;t:if(t==="style")if(typeof e=="string")n.style.cssText=e;else{if(typeof _=="string"&&(n.style.cssText=_=""),_)for(t in _)e&&t in e||mt(n.style,t,"");if(e)for(t in e)_&&e[t]===_[t]||mt(n.style,t,e[t])}else if(t[0]==="o"&&t[1]==="n")i=t!==(t=t.replace(/(PointerCapture)$|Capture$/i,"$1")),t=t.toLowerCase()in n||t==="onFocusOut"||t==="onFocusIn"?t.toLowerCase().slice(2):t.slice(2),n.l||(n.l={}),n.l[t+i]=e,e?_?e.u=_.u:(e.u=lt,n.addEventListener(t,i?rt:nt,i)):n.removeEventListener(t,i?rt:nt,i);else{if(r)t=t.replace(/xlink(H|:h)/,"h").replace(/sName$/,"s");else if(t!="width"&&t!="height"&&t!="href"&&t!="list"&&t!="form"&&t!="tabIndex"&&t!="download"&&t!="rowSpan"&&t!="colSpan"&&t!="role"&&t in n)try{n[t]=e??"";break t}catch{}typeof e=="function"||(e==null||e===!1&&t[4]!=="-"?n.removeAttribute(t):n.setAttribute(t,e))}}function gt(n){return function(t){if(this.l){var e=this.l[t.type+n];if(t.t==null)t.t=lt++;else if(t.t<e.u)return;return e(p.event?p.event(t):t)}}}function ct(n,t,e,_,r,i,s,l,c,a){var h,o,f,d,v,T,y,b,I,x,O,A,ft,$,Q,w=t.type;if(t.constructor!==void 0)return null;128&e.__u&&(c=!!(32&e.__u),i=[l=t.__e=e.__e]),(h=p.__b)&&h(t);t:if(typeof w=="function")try{if(b=t.props,I=(h=w.contextType)&&_[h.__c],x=h?I?I.props.value:h.__:_,e.__c?y=(o=t.__c=e.__c).__=o.__E:("prototype"in w&&w.prototype.render?t.__c=o=new w(b,x):(t.__c=o=new V(b,x),o.constructor=w,o.render=Gt),I&&I.sub(o),o.props=b,o.state||(o.state={}),o.context=x,o.__n=_,f=o.__d=!0,o.__h=[],o._sb=[]),o.__s==null&&(o.__s=o.state),w.getDerivedStateFromProps!=null&&(o.__s==o.state&&(o.__s=U({},o.__s)),U(o.__s,w.getDerivedStateFromProps(b,o.__s))),d=o.props,v=o.state,o.__v=t,f)w.getDerivedStateFromProps==null&&o.componentWillMount!=null&&o.componentWillMount(),o.componentDidMount!=null&&o.__h.push(o.componentDidMount);else{if(w.getDerivedStateFromProps==null&&b!==d&&o.componentWillReceiveProps!=null&&o.componentWillReceiveProps(b,x),!o.__e&&(o.shouldComponentUpdate!=null&&o.shouldComponentUpdate(b,o.__s,x)===!1||t.__v===e.__v)){for(t.__v!==e.__v&&(o.props=b,o.state=o.__s,o.__d=!1),t.__e=e.__e,t.__k=e.__k,t.__k.forEach(function(H){H&&(H.__=t)}),O=0;O<o._sb.length;O++)o.__h.push(o._sb[O]);o._sb=[],o.__h.length&&s.push(o);break t}o.componentWillUpdate!=null&&o.componentWillUpdate(b,o.__s,x),o.componentDidUpdate!=null&&o.__h.push(function(){o.componentDidUpdate(d,v,T)})}if(o.context=x,o.props=b,o.__P=n,o.__e=!1,A=p.__r,ft=0,"prototype"in w&&w.prototype.render){for(o.state=o.__s,o.__d=!1,A&&A(t),h=o.render(o.props,o.state,o.context),$=0;$<o._sb.length;$++)o.__h.push(o._sb[$]);o._sb=[]}else do o.__d=!1,A&&A(t),h=o.render(o.props,o.state,o.context),o.state=o.__s;while(o.__d&&++ft<25);o.state=o.__s,o.getChildContext!=null&&(_=U(U({},_),o.getChildContext())),f||o.getSnapshotBeforeUpdate==null||(T=o.getSnapshotBeforeUpdate(d,v)),Bt(n,at(Q=h!=null&&h.type===J&&h.key==null?h.props.children:h)?Q:[Q],t,e,_,r,i,s,l,c,a),o.base=t.__e,t.__u&=-161,o.__h.length&&s.push(o),y&&(o.__E=o.__=null)}catch(H){t.__v=null,c||i!=null?(t.__e=l,t.__u|=c?160:32,i[i.indexOf(l)]=null):(t.__e=e.__e,t.__k=e.__k),p.__e(H,t,e)}else i==null&&t.__v===e.__v?(t.__k=e.__k,t.__e=e.__e):t.__e=zt(e.__e,t,e,_,r,i,s,c,a);(h=p.diffed)&&h(t)}function $t(n,t,e){t.__d=void 0;for(var _=0;_<e.length;_++)dt(e[_],e[++_],e[++_]);p.__c&&p.__c(t,n),n.some(function(r){try{n=r.__h,r.__h=[],n.some(function(i){i.call(r)})}catch(i){p.__e(i,r.__v)}})}function zt(n,t,e,_,r,i,s,l,c){var a,h,o,f,d,v,T,y=e.props,b=t.props,I=t.type;if(I==="svg"&&(r=!0),i!=null){for(a=0;a<i.length;a++)if((d=i[a])&&"setAttribute"in d==!!I&&(I?d.localName===I:d.nodeType===3)){n=d,i[a]=null;break}}if(n==null){if(I===null)return document.createTextNode(b);n=r?document.createElementNS("http://www.w3.org/2000/svg",I):document.createElement(I,b.is&&b),i=null,l=!1}if(I===null)y===b||l&&n.data===b||(n.data=b);else{if(i=i&&Z.call(n.childNodes),y=e.props||B,!l&&i!=null)for(y={},a=0;a<n.attributes.length;a++)y[(d=n.attributes[a]).name]=d.value;for(a in y)d=y[a],a=="children"||(a=="dangerouslySetInnerHTML"?o=d:a==="key"||a in b||M(n,a,null,d,r));for(a in b)d=b[a],a=="children"?f=d:a=="dangerouslySetInnerHTML"?h=d:a=="value"?v=d:a=="checked"?T=d:a==="key"||l&&typeof d!="function"||y[a]===d||M(n,a,d,y[a],r);if(h)l||o&&(h.__html===o.__html||h.__html===n.innerHTML)||(n.innerHTML=h.__html),t.__k=[];else if(o&&(n.innerHTML=""),Bt(n,at(f)?f:[f],t,e,_,r&&I!=="foreignObject",i,s,i?i[0]:e.__k&&E(e,0),l,c),i!=null)for(a=i.length;a--;)i[a]!=null&&Rt(i[a]);l||(a="value",v!==void 0&&(v!==n[a]||I==="progress"&&!v||I==="option"&&v!==y[a])&&M(n,a,v,y[a],!1),a="checked",T!==void 0&&T!==n[a]&&M(n,a,T,y[a],!1))}return n}function dt(n,t,e){try{typeof n=="function"?n(t):n.current=t}catch(_){p.__e(_,e)}}function _t(n,t,e){var _,r;if(p.unmount&&p.unmount(n),(_=n.ref)&&(_.current&&_.current!==n.__e||dt(_,null,t)),(_=n.__c)!=null){if(_.componentWillUnmount)try{_.componentWillUnmount()}catch(i){p.__e(i,t)}_.base=_.__P=null}if(_=n.__k)for(r=0;r<_.length;r++)_[r]&&_t(_[r],t,e||typeof n.type!="function");e||n.__e==null||Rt(n.__e),n.__c=n.__=n.__e=n.__d=void 0}function Gt(n,t,e){return this.constructor(n,e)}function qt(n,t,e){var _,r,i,s;p.__&&p.__(n,t),r=(_=typeof e=="function")?null:e&&e.__k||t.__k,i=[],s=[],ct(t,n=(!_&&e||t).__k=Vt(J,null,[n]),r||B,B,t.ownerSVGElement!==void 0,!_&&e?[e]:r?null:t.firstChild?Z.call(t.childNodes):null,i,!_&&e?e:r?r.__e:t.firstChild,_,s),$t(i,n,s)}Z=At.slice,p={__e:function(n,t,e,_){for(var r,i,s;t=t.__;)if((r=t.__c)&&!r.__)try{if((i=r.constructor)&&i.getDerivedStateFromError!=null&&(r.setState(i.getDerivedStateFromError(n)),s=r.__d),r.componentDidCatch!=null&&(r.componentDidCatch(n,_||{}),s=r.__d),s)return r.__E=r}catch(l){n=l}throw n}},Et=0,V.prototype.setState=function(n,t){var e;e=this.__s!=null&&this.__s!==this.state?this.__s:this.__s=U({},this.state),typeof n=="function"&&(n=n(U({},e),this.props)),n&&U(e,n),n!=null&&this.__v&&(t&&this._sb.push(t),bt(this))},V.prototype.forceUpdate=function(n){this.__v&&(this.__e=!0,n&&this.__h.push(n),bt(this))},V.prototype.render=J,S=[],Pt=typeof Promise=="function"?Promise.prototype.then.bind(Promise.resolve()):setTimeout,et=function(n,t){return n.__v.__b-t.__v.__b},z.__r=0,lt=0,nt=gt(!1),rt=gt(!0);var Kt=0;function u(n,t,e,_,r,i){var s,l,c={};for(l in t)l=="ref"?s=t[l]:c[l]=t[l];var a={type:n,props:c,key:e,ref:s,__k:null,__:null,__b:0,__e:null,__d:void 0,__c:null,constructor:void 0,__v:--Kt,__i:-1,__u:0,__source:r,__self:i};if(typeof n=="function"&&(s=n.defaultProps))for(l in s)c[l]===void 0&&(c[l]=s[l]);return p.vnode&&p.vnode(a),a}function F({...n}){return u("div",{className:"variable",style:{display:"flex",justifyContent:"space-around",flexDirection:"row",gap:"8px"},children:[n.title&&u("div",{children:u("strong",{children:[n.title,": "]})}),u("div",{children:u("em",{children:[n.value," ",n.unit]})})]})}function vt({...n}){return u("div",{style:{padding:"10px 32px",width:"200px"},children:[u("h5",{children:n.title}),u("ul",{style:{listStyleType:"none",padding:0},children:[u("li",{children:u(F,{title:"Temp",value:n.temperature,unit:"°C"})}),u("li",{children:u(F,{title:"RPM",value:n.rpm,unit:"RPM"})}),u("li",{children:u(F,{title:"Angle",value:n.angle,unit:"°"})})]})]})}function P({children:n,...t}){return u("div",{style:{display:"flex",padding:"12px",flexDirection:"row",gap:"8px",width:"100%",justifyContent:"space-around",alignItems:"center"},children:n})}function Xt({children:n,...t}){return u("div",{style:{display:"flex",padding:"12px",flexDirection:"column",gap:"4px",justifyContent:"space-around",alignItems:"center",width:"95%"},children:n})}var G,m,tt,yt,it=0,Ht=[],W=[],g=p,It=g.__b,Ft=g.__r,wt=g.diffed,Tt=g.__c,kt=g.unmount,xt=g.__;function Mt(n,t){g.__h&&g.__h(m,n,it||t),it=0;var e=m.__H||(m.__H={__:[],__h:[]});return n>=e.__.length&&e.__.push({__V:W}),e.__[n]}function Yt(n){return it=1,Zt(Lt,n)}function Zt(n,t,e){var _=Mt(G++,2);if(_.t=n,!_.__c&&(_.__=[e?e(t):Lt(void 0,t),function(l){var c=_.__N?_.__N[0]:_.__[0],a=_.t(c,l);c!==a&&(_.__N=[a,_.__[1]],_.__c.setState({}))}],_.__c=m,!m.u)){var r=function(l,c,a){if(!_.__c.__H)return!0;var h=_.__c.__H.__.filter(function(f){return!!f.__c});if(h.every(function(f){return!f.__N}))return!i||i.call(this,l,c,a);var o=!1;return h.forEach(function(f){if(f.__N){var d=f.__[0];f.__=f.__N,f.__N=void 0,d!==f.__[0]&&(o=!0)}}),!(!o&&_.__c.props===l)&&(!i||i.call(this,l,c,a))};m.u=!0;var i=m.shouldComponentUpdate,s=m.componentWillUpdate;m.componentWillUpdate=function(l,c,a){if(this.__e){var h=i;i=void 0,r(l,c,a),i=h}s&&s.call(this,l,c,a)},m.shouldComponentUpdate=r}return _.__N||_.__}function Jt(n,t){var e=Mt(G++,3);!g.__s&&ee(e.__H,t)&&(e.__=n,e.i=t,m.__H.__h.push(e))}function Qt(){for(var n;n=Ht.shift();)if(n.__P&&n.__H)try{n.__H.__h.forEach(j),n.__H.__h.forEach(ot),n.__H.__h=[]}catch(t){n.__H.__h=[],g.__e(t,n.__v)}}g.__b=function(n){m=null,It&&It(n)},g.__=function(n,t){n&&t.__k&&t.__k.__m&&(n.__m=t.__k.__m),xt&&xt(n,t)},g.__r=function(n){Ft&&Ft(n),G=0;var t=(m=n.__c).__H;t&&(tt===m?(t.__h=[],m.__h=[],t.__.forEach(function(e){e.__N&&(e.__=e.__N),e.__V=W,e.__N=e.i=void 0})):(t.__h.forEach(j),t.__h.forEach(ot),t.__h=[],G=0)),tt=m},g.diffed=function(n){wt&&wt(n);var t=n.__c;t&&t.__H&&(t.__H.__h.length&&(Ht.push(t)!==1&&yt===g.requestAnimationFrame||((yt=g.requestAnimationFrame)||te)(Qt)),t.__H.__.forEach(function(e){e.i&&(e.__H=e.i),e.__V!==W&&(e.__=e.__V),e.i=void 0,e.__V=W})),tt=m=null},g.__c=function(n,t){t.some(function(e){try{e.__h.forEach(j),e.__h=e.__h.filter(function(_){return!_.__||ot(_)})}catch(_){t.some(function(r){r.__h&&(r.__h=[])}),t=[],g.__e(_,e.__v)}}),Tt&&Tt(n,t)},g.unmount=function(n){kt&&kt(n);var t,e=n.__c;e&&e.__H&&(e.__H.__.forEach(function(_){try{j(_)}catch(r){t=r}}),e.__H=void 0,t&&g.__e(t,e.__v))};var Nt=typeof requestAnimationFrame=="function";function te(n){var t,e=function(){clearTimeout(_),Nt&&cancelAnimationFrame(t),setTimeout(n)},_=setTimeout(e,100);Nt&&(t=requestAnimationFrame(e))}function j(n){var t=m,e=n.__c;typeof e=="function"&&(n.__c=void 0,e()),m=t}function ot(n){var t=m;n.__c=n.__(),m=t}function ee(n,t){return!n||n.length!==t.length||t.some(function(e,_){return e!==n[_]})}function Lt(n,t){return typeof t=="function"?t(n):t}const R=4,C=4,ut=4,N=new Int32Array(2),Ut=new Float32Array(N.buffer),St=new Float64Array(N.buffer),L=new Uint16Array(new Uint8Array([1,0]).buffer)[0]===1;var st;(function(n){n[n.UTF8_BYTES=1]="UTF8_BYTES",n[n.UTF16_STRING=2]="UTF16_STRING"})(st||(st={}));class ht{constructor(t){this.bytes_=t,this.position_=0,this.text_decoder_=new TextDecoder}static allocate(t){return new ht(new Uint8Array(t))}clear(){this.position_=0}bytes(){return this.bytes_}position(){return this.position_}setPosition(t){this.position_=t}capacity(){return this.bytes_.length}readInt8(t){return this.readUint8(t)<<24>>24}readUint8(t){return this.bytes_[t]}readInt16(t){return this.readUint16(t)<<16>>16}readUint16(t){return this.bytes_[t]|this.bytes_[t+1]<<8}readInt32(t){return this.bytes_[t]|this.bytes_[t+1]<<8|this.bytes_[t+2]<<16|this.bytes_[t+3]<<24}readUint32(t){return this.readInt32(t)>>>0}readInt64(t){return BigInt.asIntN(64,BigInt(this.readUint32(t))+(BigInt(this.readUint32(t+4))<<BigInt(32)))}readUint64(t){return BigInt.asUintN(64,BigInt(this.readUint32(t))+(BigInt(this.readUint32(t+4))<<BigInt(32)))}readFloat32(t){return N[0]=this.readInt32(t),Ut[0]}readFloat64(t){return N[L?0:1]=this.readInt32(t),N[L?1:0]=this.readInt32(t+4),St[0]}writeInt8(t,e){this.bytes_[t]=e}writeUint8(t,e){this.bytes_[t]=e}writeInt16(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8}writeUint16(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8}writeInt32(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8,this.bytes_[t+2]=e>>16,this.bytes_[t+3]=e>>24}writeUint32(t,e){this.bytes_[t]=e,this.bytes_[t+1]=e>>8,this.bytes_[t+2]=e>>16,this.bytes_[t+3]=e>>24}writeInt64(t,e){this.writeInt32(t,Number(BigInt.asIntN(32,e))),this.writeInt32(t+4,Number(BigInt.asIntN(32,e>>BigInt(32))))}writeUint64(t,e){this.writeUint32(t,Number(BigInt.asUintN(32,e))),this.writeUint32(t+4,Number(BigInt.asUintN(32,e>>BigInt(32))))}writeFloat32(t,e){Ut[0]=e,this.writeInt32(t,N[0])}writeFloat64(t,e){St[0]=e,this.writeInt32(t,N[L?0:1]),this.writeInt32(t+4,N[L?1:0])}getBufferIdentifier(){if(this.bytes_.length<this.position_+R+C)throw new Error("FlatBuffers: ByteBuffer is too short to contain an identifier.");let t="";for(let e=0;e<C;e++)t+=String.fromCharCode(this.readInt8(this.position_+R+e));return t}__offset(t,e){const _=t-this.readInt32(t);return e<this.readInt16(_)?this.readInt16(_+e):0}__union(t,e){return t.bb_pos=e+this.readInt32(e),t.bb=this,t}__string(t,e){t+=this.readInt32(t);const _=this.readInt32(t);t+=R;const r=this.bytes_.subarray(t,t+_);return e===st.UTF8_BYTES?r:this.text_decoder_.decode(r)}__union_with_string(t,e){return typeof t=="string"?this.__string(e):this.__union(t,e)}__indirect(t){return t+this.readInt32(t)}__vector(t){return t+this.readInt32(t)+R}__vector_len(t){return this.readInt32(t+this.readInt32(t))}__has_identifier(t){if(t.length!=C)throw new Error("FlatBuffers: file identifier must be length "+C);for(let e=0;e<C;e++)if(t.charCodeAt(e)!=this.readInt8(this.position()+R+e))return!1;return!0}createScalarList(t,e){const _=[];for(let r=0;r<e;++r){const i=t(r);i!==null&&_.push(i)}return _}createObjList(t,e){const _=[];for(let r=0;r<e;++r){const i=t(r);i!==null&&_.push(i.unpack())}return _}}var q=(n=>(n[n.Manual=0]="Manual",n[n.FlightController=1]="FlightController",n[n.OnboardComputer=2]="OnboardComputer",n[n.RC=3]="RC",n))(q||{});class k{constructor(){this.bb=null,this.bb_pos=0}__init(t,e){return this.bb_pos=t,this.bb=e,this}static getRootAsMotor(t,e){return(e||new k).__init(t.readInt32(t.position())+t.position(),t)}static getSizePrefixedRootAsMotor(t,e){return t.setPosition(t.position()+ut),(e||new k).__init(t.readInt32(t.position())+t.position(),t)}temperature(){const t=this.bb.__offset(this.bb_pos,4);return t?this.bb.readFloat32(this.bb_pos+t):0}rps(){const t=this.bb.__offset(this.bb_pos,6);return t?this.bb.readFloat32(this.bb_pos+t):0}angle(){const t=this.bb.__offset(this.bb_pos,8);return t?this.bb.readFloat32(this.bb_pos+t):0}static startMotor(t){t.startObject(3)}static addTemperature(t,e){t.addFieldFloat32(0,e,0)}static addRps(t,e){t.addFieldFloat32(1,e,0)}static addAngle(t,e){t.addFieldFloat32(2,e,0)}static endMotor(t){return t.endObject()}static createMotor(t,e,_,r){return k.startMotor(t),k.addTemperature(t,e),k.addRps(t,_),k.addAngle(t,r),k.endMotor(t)}}class K{constructor(){this.bb=null,this.bb_pos=0}__init(t,e){return this.bb_pos=t,this.bb=e,this}static getRootAsMotors(t,e){return(e||new K).__init(t.readInt32(t.position())+t.position(),t)}static getSizePrefixedRootAsMotors(t,e){return t.setPosition(t.position()+ut),(e||new K).__init(t.readInt32(t.position())+t.position(),t)}motor1(t){const e=this.bb.__offset(this.bb_pos,4);return e?(t||new k).__init(this.bb.__indirect(this.bb_pos+e),this.bb):null}motor2(t){const e=this.bb.__offset(this.bb_pos,6);return e?(t||new k).__init(this.bb.__indirect(this.bb_pos+e),this.bb):null}static startMotors(t){t.startObject(2)}static addMotor1(t,e){t.addFieldOffset(0,e,0)}static addMotor2(t,e){t.addFieldOffset(1,e,0)}static endMotors(t){return t.endObject()}}var X=(n=>(n[n.OK=0]="OK",n[n.INIT=1]="INIT",n[n.NOTX=2]="NOTX",n[n.BAT=3]="BAT",n[n.TEMP=4]="TEMP",n[n.OFF=5]="OFF",n))(X||{});class Y{constructor(){this.bb=null,this.bb_pos=0}__init(t,e){return this.bb_pos=t,this.bb=e,this}static getRootAsRobot(t,e){return(e||new Y).__init(t.readInt32(t.position())+t.position(),t)}static getSizePrefixedRootAsRobot(t,e){return t.setPosition(t.position()+ut),(e||new Y).__init(t.readInt32(t.position())+t.position(),t)}batteries(){const t=this.bb.__offset(this.bb_pos,4);return t?this.bb.readInt32(this.bb_pos+t):4}controlSource(){const t=this.bb.__offset(this.bb_pos,6);return t?this.bb.readInt8(this.bb_pos+t):q.RC}status(){const t=this.bb.__offset(this.bb_pos,8);return t?this.bb.readInt8(this.bb_pos+t):X.OK}motors(t){const e=this.bb.__offset(this.bb_pos,10);return e?(t||new K).__init(this.bb.__indirect(this.bb_pos+e),this.bb):null}voltage(){const t=this.bb.__offset(this.bb_pos,12);return t?this.bb.readFloat32(this.bb_pos+t):0}current(){const t=this.bb.__offset(this.bb_pos,14);return t?this.bb.readFloat32(this.bb_pos+t):0}fuel(){const t=this.bb.__offset(this.bb_pos,16);return t?this.bb.readFloat32(this.bb_pos+t):0}rssi(){const t=this.bb.__offset(this.bb_pos,18);return t?this.bb.readFloat32(this.bb_pos+t):0}signalStrength(){const t=this.bb.__offset(this.bb_pos,20);return t?this.bb.readInt32(this.bb_pos+t):0}maxSpeed(){const t=this.bb.__offset(this.bb_pos,22);return t?this.bb.readFloat32(this.bb_pos+t):0}lowVoltageThreshold(){const t=this.bb.__offset(this.bb_pos,24);return t?this.bb.readFloat32(this.bb_pos+t):12}criticalVoltageThreshold(){const t=this.bb.__offset(this.bb_pos,26);return t?this.bb.readFloat32(this.bb_pos+t):16}referenceWheelAngle(){const t=this.bb.__offset(this.bb_pos,28);return t?this.bb.readInt32(this.bb_pos+t):0}motorErrorCode(t){const e=this.bb.__offset(this.bb_pos,30);return e?this.bb.__string(this.bb_pos+e,t):null}wheelsFolded(){const t=this.bb.__offset(this.bb_pos,32);return t?!!this.bb.readInt8(this.bb_pos+t):!1}enableRover(){const t=this.bb.__offset(this.bb_pos,34);return t?!!this.bb.readInt8(this.bb_pos+t):!1}static startRobot(t){t.startObject(16)}static addBatteries(t,e){t.addFieldInt32(0,e,4)}static addControlSource(t,e){t.addFieldInt8(1,e,q.RC)}static addStatus(t,e){t.addFieldInt8(2,e,X.OK)}static addMotors(t,e){t.addFieldOffset(3,e,0)}static addVoltage(t,e){t.addFieldFloat32(4,e,0)}static addCurrent(t,e){t.addFieldFloat32(5,e,0)}static addFuel(t,e){t.addFieldFloat32(6,e,0)}static addRssi(t,e){t.addFieldFloat32(7,e,0)}static addSignalStrength(t,e){t.addFieldInt32(8,e,0)}static addMaxSpeed(t,e){t.addFieldFloat32(9,e,0)}static addLowVoltageThreshold(t,e){t.addFieldFloat32(10,e,12)}static addCriticalVoltageThreshold(t,e){t.addFieldFloat32(11,e,16)}static addReferenceWheelAngle(t,e){t.addFieldInt32(12,e,0)}static addMotorErrorCode(t,e){t.addFieldOffset(13,e,0)}static addWheelsFolded(t,e){t.addFieldInt8(14,+e,0)}static addEnableRover(t,e){t.addFieldInt8(15,+e,0)}static endRobot(t){return t.endObject()}static finishRobotBuffer(t,e){t.finish(e)}static finishSizePrefixedRobotBuffer(t,e){t.finish(e,void 0,!0)}}const ne=n=>({motors:{motor1:{temperature:n.motors().motor1().temperature(),rps:n.motors().motor1().rps(),angle:n.motors().motor1().angle()},motor2:{temperature:n.motors().motor2().temperature(),rps:n.motors().motor2().rps(),angle:n.motors().motor2().angle()}},batteries:n.batteries(),control_source:q[n.controlSource()],status:X[n.status()],voltage:n.voltage(),current:n.current(),rssi:n.rssi(),signalStrength:n.signalStrength(),max_speed:n.maxSpeed(),low_voltage_threshold:n.lowVoltageThreshold(),critical_voltage_threshold:n.criticalVoltageThreshold(),reference_wheel_angle:n.referenceWheelAngle(),motor_error_code:n.motorErrorCode(),wheels_folded:n.wheelsFolded(),enable_rover:n.enableRover()});function re(){const[n,t]=Yt({motors:{motor1:{temperature:22,rpm:40,angle:3},motor2:{temperature:24,rpm:50,angle:3}},batteries:{number:4,status:"NOT_X"},controlSource:"RC",fuel:50,voltage:12,current:2,rssi:-50,signalStrength:-50,offset:{left:0,right:0}});return Jt(()=>{const e=new WebSocket("ws://localhost:8080");return e.onopen=()=>{console.log("Connected to server")},e.onmessage=_=>{console.log(_);const r=new FileReader;r.readAsArrayBuffer(_.data),r.onload=()=>{const i=new ht(new Uint8Array(r.result)),s=Y.getRootAsRobot(i);console.log(ne(s))}},()=>{e.close()}},[]),u(Xt,{children:[u(vt,{title:"Motor 1",temperature:n.motors.motor1.temperature,rpm:n.motors.motor1.rpm,angle:n.motors.motor1.angle}),u(vt,{title:"Motor 2",temperature:n.motors.motor2.temperature,rpm:n.motors.motor2.rpm,angle:n.motors.motor2.angle}),u(P,{children:[u(F,{title:"No. Bats",value:n.batteries.number}),u(F,{title:"Status",value:n.batteries.status})]}),u(P,{children:u(F,{title:"Control Source",value:n.controlSource})}),u(P,{children:u(F,{title:"Fuel",value:n.fuel,unit:"%"})}),u(P,{children:[u(F,{title:"Voltage",value:n.voltage,unit:"V"}),u(F,{title:"Current",value:n.current,unit:"A"})]}),u(P,{children:[u(F,{title:"RSSI",value:n.rssi,unit:"dBm"}),u(F,{title:"Signal",value:n.signalStrength,unit:"dBm"})]}),u(P,{children:[u(F,{title:"Offset L",value:n.offset.left,unit:"°"}),u(F,{title:"Offset R",value:n.offset.right,unit:"°"})]}),u("button",{children:"Test Update"})]})}qt(u(re,{}),document.getElementById("app"));
