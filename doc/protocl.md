1. 注册请求

tcp 无状态 请求

tcp 无状态 响应

REGISTER sip:ims.mnc001.mcc001.3gppnetwork.org SIP/2.0
To: <sip:001010123456712@ims.mnc001.mcc001.3gppnetwork.org>
From: <sip:001010123456712@ims.mnc001.mcc001.3gppnetwork.org>;tag=W8dcbT3
Contact: <sip:001010123456712@182.0.3.112:5060>;+sip.instance="<urn:gsma:imei:86130905-267370-0>";+g.3gpp.accesstype="cellular2";audio;video;+g.3gpp.smsip;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
Expires: 600000
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Supported: path,sec-agree
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,PRACK,NOTIFY,MESSAGE,REFER
Require: sec-agree
Proxy-Require: sec-agree
Security-Client: ipsec-3gpp;alg=hmac-sha-1-96;prot=esp;mod=trans;ealg=des-ede3-cbc;spi-c=13710035;spi-s=50757030;port-c=31521;port-s=31867,ipsec-3gpp;alg=hmac-sha-1-96;prot=esp;mod=trans;ealg=aes-cbc;spi-c=13710035;spi-s=50757030;port-c=31521;port-s=31867,ipsec-3gpp;alg=hmac-sha-1-96;prot=esp;mod=trans;ealg=null;spi-c=13710035;spi-s=50757030;port-c=31521;port-s=31867,ipsec-3gpp;alg=hmac-md5-96;prot=esp;mod=trans;ealg=des-ede3-cbc;spi-c=13710035;spi-s=50757030;port-c=31521;port-s=31867,ipsec-3gpp;alg=hmac-md5-96;prot=esp;mod=trans;ealg=aes-cbc;spi-c=13710035;spi-s=50757030;port-c=31521;port-s=31867,ipsec-3gpp;alg=hmac-md5-96;prot=esp;mod=trans;ealg=null;spi-c=13710035;spi-s=50757030;port-c=31521;port-s=31867
Authorization: Digest username="001010123456712@ims.mnc001.mcc001.3gppnetwork.org",realm="ims.mnc001.mcc001.3gppnetwork.org",uri="sip:ims.mnc001.mcc001.3gppnetwork.org",nonce="",response=""
Call-ID: Z7dcb95rP@182.0.3.112
CSeq: 1 REGISTER
Max-Forwards: 70
Via: SIP/2.0/TCP 182.0.3.112:5060;branch=z9hG4bKX9dcbT3QIWzL-OaaaW2Y;rport
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Length: 0

SIP/2.0 200 OK
From: <sip:001010123456712@ims.mnc001.mcc001.3gppnetwork.org>;tag=W8dcbT3
To: <sip:001010123456712@ims.mnc001.mcc001.3gppnetwork.org>;tag=36028ef7-527687b4-19577a-7fa7f620d678-2278580a-13c4-7217 // 添加tag
Call-ID: Z7dcb95rP@182.0.3.112
CSeq: 1 REGISTER
Via: SIP/2.0/TCP 182.0.3.112:5060;rport=5060;branch=z9hG4bKX9dcbT3QIWzL-OaaaW2Y // 去掉了;rport
Contact: <sip:001010123456712@182.0.3.112:5060>;expires=600000;audio;video;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel";+g.3gpp.smsip="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel";+sip.instance="<urn:gsma:imei:86130905-267370-0>";+g.3gpp.accesstype="cellular2"
// 大改，但是也是来自于UE带来的信息
P-Associated-URI: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>,<tel:1012> // 新
Content-Length: 0

2. 订阅请求

udp 无状态 请求

udp 无状态 响应

udp 无状态 响应

SUBSCRIBE sip:1012@ims.mnc001.mcc001.3gppnetwork.org SIP/2.0
From: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>;tag=Z1dcbT3
To: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Contact: <sip:1012@182.0.3.112:5060>;+sip.instance="<urn:gsma:imei:86130905-267370-0>"
Supported: path,sec-agree
Event: reg
Accept: application/reginfo+xml
Expires: 600000
Route: <sip:10.88.120.34:5060;lr>
Require: sec-agree
Proxy-Require: sec-agree
Call-ID: Y-dcbT3QI@182.0.3.112
CSeq: 1 SUBSCRIBE
Max-Forwards: 70
Via: SIP/2.0/UDP 182.0.3.112:5060;branch=z9hG4bKaaecbT3QIWzL-OaaaW2Y;rport
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Length: 0

SIP/2.0 200 OK
From: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>;tag=Z1dcbT3
To: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>;tag=4315e455-b855d83-19577b-7fa7f620d928-2278580a-13c4-7217  // 添加tag
Call-ID: Y-dcbT3QI@182.0.3.112
CSeq: 1 SUBSCRIBE
Expires: 600000
Via: SIP/2.0/UDP 182.0.3.112:5060;rport=5060;branch=z9hG4bKaaecbT3QIWzL-OaaaW2Y   // 中间添加了rport
Contact: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>  // 大改，几乎是重新构造，但是可以直接引用TO的内容
Content-Length: 0

NOTIFY sip:1012@182.0.3.112:5060 SIP/2.0
From: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>;tag=4315e455-b855d83-19577b-7fa7f620d928-2278580a-13c4-7217
To: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>;tag=Z1dcbT3
Call-ID: Y-dcbT3QI@182.0.3.112
CSeq: 1 NOTIFY
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-418a3fd9-19577b-62fdb882-7fa7f6a469d8
Subscription-State: active;expires=599999
Event: reg
Max-Forwards: 70
Contact: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
Content-Length: 0



3. notify响应

udp 无状态 响应

无动于衷。。。


4. invite请求

tcp 有状态 UE1请求

tcp 有状态 响应UE1

udp 有状态 请求UE2

INVITE tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org SIP/2.0
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>
P-Preferred-Identity: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>
Contact: <sip:1013@182.0.3.113:5060>;+sip.instance="<urn:gsma:imei:86728604-423716-0>";+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel";audio;video;+g.3gpp.mid-call;+g.3gpp.srvcc-alerting;+g.3gpp.ps2cs-srvcc-orig-pre-alerting
Accept-Contact: *;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
P-Preferred-Service: urn:urn-7:3gpp-service.ims.icsi.mmtel
P-Early-Media: supported
Supported: 100rel,histinfo,join,norefersub,precondition,replaces,timer,sec-agree
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
Accept: application/sdp,application/3gpp-ims+xml
Session-Expires: 1800
Min-SE: 90
Route: <sip:10.88.120.34:5060;lr>
Require: sec-agree
Proxy-Require: sec-agree
Call-ID: ttecbbPKc@182.0.3.113
CSeq: 1 INVITE
Max-Forwards: 70
Via: SIP/2.0/TCP 182.0.3.113:5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY;rport
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Type: application/sdp
Content-Length: 770

SIP/2.0 100 Trying
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>
Call-ID: ttecbbPKc@182.0.3.113
CSeq: 1 INVITE
Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY  // 添加上了rport
Contact: <sip:10.88.120.34:5060;transport=UDP>           // 自己进行构造
Content-Length: 0

INVITE sip:1012@182.0.3.112:5060 SIP/2.0        // 自己构造，找到对方的ID也可以找到
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: <sip:1012@182.0.3.112:5060>                   // 自己构造
Call-ID: 28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-7217  // 自己构造而且以后会反复用到这个相同的ID
CSeq: 1 INVITE
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-65502eb3-195782-62fdd60c-7fa7f6a474f0  // 自己构造，不知道以后会不会用到
P-Preferred-Identity: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>
Accept-Contact: *;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
P-Preferred-Service: urn:urn-7:3gpp-service.ims.icsi.mmtel
Accept: application/sdp,application/3gpp-ims+xml
Require: sec-agree
Proxy-Require: sec-agree
Max-Forwards: 70
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
P-Early-Media: supported
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
Session-Expires: 1800
Min-SE: 90
Contact: <sip:10.88.120.34:5060;transport=UDP> // 自己构造
Content-Type: application/sdp
Content-Length: 770



5. 183 progress 请求

udp 有状态 UE2请求

tcp 有状态 响应UE1


SIP/2.0 183 Session Progress
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Contact: <sip:1012@182.0.3.112:5060>;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel";audio;video;+g.3gpp.mid-call;+g.3gpp.srvcc-alerting
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
Call-ID: 28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-7217
CSeq: 1 INVITE
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-65502eb3-195782-62fdd60c-7fa7f6a474f0
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Type: application/sdp
Content-Length: 335

SIP/2.0 183 Session Progress
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217  // 修改to的tag变成自己、、而且这个内容等同于自己再构造一遍
Call-ID: ttecbbPKc@182.0.3.113                               // 直接替换掉call-ID
CSeq: 1 INVITE
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY                  // 修改via，，更换branch参数值，貌似这一个值气态地方都用不到
Contact: <sip:10.88.120.34:5060;transport=UDP>  // 自己构造
Content-Type: application/sdp
Content-Length: 335

6. 180 ring 请求

udp 有状态 UE2请求
tcp 有状态 请求UE1

SIP/2.0 180 Ringing
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Contact: <sip:1012@182.0.3.112:5060>;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel";audio;video;+g.3gpp.mid-call;+g.3gpp.srvcc-alerting
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
Call-ID: 28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-7217
CSeq: 1 INVITE
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-65502eb3-195782-62fdd60c-7fa7f6a474f0
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Length: 0

SIP/2.0 180 Ringing
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
																	     6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217 // 修改to的tag变成自己、、而且这个内容等同于自己再构造一遍
Call-ID: ttecbbPKc@182.0.3.113         // 修改call-ID
CSeq: 1 INVITE             
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY  // 自己构造、这个值可能是唯一的
Contact: <sip:10.88.120.34:5060;transport=UDP>                                // 自己构造
Content-Length: 0

7. 200 OK 响应

udp 有状态 UE2请求
tcp 有状态 请求UE1

// 此时UE1的TCP连接，之后没有tcp消息了？？

SIP/2.0 200 OK
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Contact: <sip:1012@182.0.3.112:5060>;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel";audio;video;+g.3gpp.mid-call;+g.3gpp.srvcc-alerting
Supported: 100rel,histinfo,join,norefersub,replaces,timer
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
Call-ID: 28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-7217
CSeq: 1 INVITE
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-65502eb3-195782-62fdd60c-7fa7f6a474f0
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Type: application/sdp
Content-Length: 335

SIP/2.0 200 OK
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217  // 修改to的tag
Call-ID: ttecbbPKc@182.0.3.113     // 修改call-ID
CSeq: 1 INVITE
P-Preferred-Identity: <sip:1012@ims.mnc001.mcc001.3gppnetwork.org>
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Supported: 100rel,histinfo,join,norefersub,replaces,timer
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Allow: INVITE,ACK,OPTIONS,BYE,CANCEL,UPDATE,INFO,PRACK,NOTIFY,MESSAGE,REFER
Via: SIP/2.0/TCP 182.0.3.113:5060;rport=5060;branch=z9hG4bKrvecbbPKcWj1EbefaynY   // 自己构造、这个值可能是随机的
Contact: <sip:10.88.120.34:5060;transport=UDP>    // 随机构造
Content-Type: application/sdp
Content-Length: 335

8. ACK 请求

udp 有状态 UE1请求

udp 有状态 请求UE2

ACK sip:10.88.120.34:5060;transport=udp SIP/2.0
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217
Contact: <sip:1013@182.0.3.113:5060>;+g.3gpp.icsi-ref="urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
Supported: 100rel,histinfo,join,norefersub,precondition,replaces,timer,sec-agree
Require: sec-agree
Proxy-Require: sec-agree
Call-ID: ttecbbPKc@182.0.3.113
CSeq: 1 ACK
Max-Forwards: 70
Via: SIP/2.0/UDP 182.0.3.113:5060;branch=z9hG4bKswecbrwxIa2O-defaSIa;rport
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Length: 0

ACK sip:1012@182.0.3.112:5060 SIP/2.0   // 自己构造
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP 
To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3    // 自己构造
Call-ID: 28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-7217  // 自己加的
CSeq: 1 ACK
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-53470c5e-195786-62fde73c-7fa7f6a47728  // 自己构造
Supported: 100rel,histinfo,join,norefersub,precondition,replaces,timer,sec-agree
Require: sec-agree
Proxy-Require: sec-agree
Max-Forwards: 70
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Contact: <sip:10.88.120.34:5060;transport=UDP>  // 自己构造
Content-Length: 0

9. BYE 请求

udp 有状态? ue1请求
udp 有状态？请求UE2

BYE sip:10.88.120.34:5060;transport=udp SIP/2.0
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217
Reason: SIP;cause=200;text="User term the call."
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Supported: 100rel,histinfo,join,norefersub,precondition,replaces,timer,sec-agree
Require: sec-agree
Proxy-Require: sec-agree
Call-ID: ttecbbPKc@182.0.3.113
CSeq: 2 BYE
Max-Forwards: 70
Via: SIP/2.0/UDP 182.0.3.113:5060;branch=z9hG4bKtxecbrwxIa2O-defaGnY;rport
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Length: 0


BYE sip:1012@182.0.3.112:5060 SIP/2.0
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
Call-ID: 28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-7217
CSeq: 2 BYE
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-fcd12d5-19578b-62fdf858-7fa7f6a47b98
Max-Forwards: 70
Content-Length: 0

10， 200 OK

udp 有状态？ UE2响应
udp 有状态？ 响应UE1

SIP/2.0 200 OK
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: <sip:1012@182.0.3.112:5060>;tag=bbecbT3
Call-ID: 28344237-58da32bd-195782-7fa7f5c942b0-2278580a-13c4-7217
CSeq: 2 BYE
Supported: 100rel,histinfo,join,norefersub,precondition,replaces,timer
P-Access-Network-Info: 3GPP-NR-TDD;utran-cell-id-3gpp=00101003132000134001
Via: SIP/2.0/UDP 10.88.120.34:5060;branch=z9hG4bK-fcd12d5-19578b-62fdf858-7fa7f6a47b98
User-Agent: IM-client/OMA1.0 HW-Rto/V1.0
Content-Length: 0

SIP/2.0 200 OK
From: <sip:1013@ims.mnc001.mcc001.3gppnetwork.org>;tag=quecbbP
To: "1012"<tel:1012;phone-context=ims.mnc001.mcc001.3gppnetwork.org>;tag=6496d2c0-29d17b12-195782-7fa7f620e698-2278580a-13c4-7217
Call-ID: ttecbbPKc@182.0.3.113
CSeq: 2 BYE
Via: SIP/2.0/UDP 182.0.3.113:5060;rport=5060;branch=z9hG4bKtxecbrwxIa2O-defaGnY
Content-Length: 0




