#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid     = "WiFi Rover";
const char *password = "helloworld";

int leftM1  = D0;
int leftM2  = D1;
int rightM1 = D2;
int rightM2 = D3;

ESP8266WebServer server(80);

void handleRoot() {
	server.send(200, "text/html",
		"<!DOCTYPE html>\n"
		"<html>\n"
		"\t<head>\n"
		"\t\t<title>ESP8266 Robot</title>\n"
		"\t\t<style type=\"text/css\">\n"
		"\t\t\t* {\n"
		"\t\t\t\tbackground: #13232f5c;\n"
		"\t\t\t\tborder: 0;\n"
		"\t\t\t\tbox-sizing: border-box;\n"
		"\t\t\t\tmargin: 0;\n"
		"\t\t\t\tpadding: 0;\n"
		"\t\t\t}\n"
		"\n"
		"\t\t\t#joystick-main {\n"
		"\t\t\t\talign-items: center;\n"
		"\t\t\t\tdisplay: flex;\n"
		"\t\t\t\theight: 100vh;\n"
		"\t\t\t\tjustify-content: center;\n"
		"\t\t\t\tmargin: auto;\n"
		"\t\t\t\twidth: 100vw;\n"
		"\t\t\t}\n"
		"\n"
		"\t\t\t#joystick-base {\n"
		"\t\t\t\tbottom: 64px;\n"
		"\t\t\t\tleft: 64px;\t\t\t\t\n"
		"\t\t\t\theight: 200px;\n"
		"\t\t\t\twidth: 200px;\n"
		"\t\t\t\tborder-radius: 100%;\n"
		"\t\t\t\tborder: 10px solid rgba(0, 136, 204, 0.1);\n"
		"\t\t\t\tbackground: rgba(0, 136, 204, 0.05);\n"
		"\t\t\t\tbox-shadow: 0 0 15px rgba(0, 0, 0, 0.5) inset, 0 0 5px rgba(0, 0, 0, 0.2);\n"
		"\t\t\t\ttransition: border-color 0.3s;\n"
		"\t\t\t\tcursor: pointer;\n"
		"\t\t\t\ttouch-action: none;\n"
		"\t\t\t}\n"
		"\t\t\t@media only screen and (max-device-width: 320px) {\n"
		"\t\t\t\t#joystick-base {\n"
		"\t\t\t\t\tmargin-top: -8.5vh;\n"
		"\t\t\t\t}\n"
		"\t\t\t}\n"
		"\t\t\t@media only screen and (max-device-width: 480px) {\n"
		"\t\t\t\t#joystick-base {\n"
		"\t\t\t\t\tmargin-top: -8.5vh;\n"
		"\t\t\t\t}\n"
		"\t\t\t}\n"
		"\t\t\t.joystick-stick {\n"
		"\t\t\t\tposition: absolute;\n"
		"\t\t\t\ttop: calc(50% - 64px);\n"
		"\t\t\t\tleft: calc(50% - 64px);\n"
		"\t\t\t\theight: 128px;\n"
		"\t\t\t\twidth: 128px;\n"
		"\t\t\t\tborder-radius: 100%;\n"
		"\t\t\t\tbackground: rgba(0, 136, 204, 0.25);\n"
		"\t\t\t\tbox-shadow: 0 0 5px rgba(0, 0, 0, 0.7) inset;\n"
		"\t\t\t\ttransition: background 0.3s;\n"
		"\t\t\t\twill-change: transform;\n"
		"\t\t\t\ttouch-action: none;\n"
		"\t\t\t};\n"
		"\t\t</style>\n"
		"\t</head>\n"
		"\n"
		"\t<body>\n"
		"\n"
		"\t<div id=\"joystick-main\">\n"
		"\t\t<div id=\"joystick-base\"></div>\n"
		"\t</div>\n"
		"\t\n"
		"\t<script type=\"text/javascript\">\n"
		"\t\tfunction JOYSTICK( parent ) {\n"
		"\t\t\tthis.dragStart = null;\n"
		"\t\t\tthis.currentPos = { x: 0, y: 0 };\n"
		"\t\t\tthis.maxDiff = 32;\n"
		"\t\t\tthis.stick = document.createElement('div');\n"
		"\t\t\tthis.stick.classList.add('joystick-stick');\n"
		"\t\t\tparent.appendChild( this.stick );\n"
		"\n"
		"\t\t\tthis.stick.addEventListener( 'mousedown', this.handleMouseDown.bind( this ) ); \n"
		"\t\t\tdocument.addEventListener('mousemove', this.handleMouseMove.bind( this ) );\n"
		"\t\t\tdocument.addEventListener('mouseup', this.handleMouseUp.bind( this ) );\n"
		"\t\t\tthis.stick.addEventListener('touchstart', this.handleMouseDown.bind( this) );\n"
		"\t\t\tdocument.addEventListener('touchmove', this.handleMouseMove.bind( this ) );\n"
		"\t\t\tdocument.addEventListener('touchend', this.handleMouseUp.bind( this ) );\n"
		"\n"
		"\t};\n"
		"\n"
		"\tJOYSTICK.prototype.handleMouseDown = function( event ) {\n"
		"\t\tthis.stick.style.transition = '0s';\n"
		"\t\tif (event.changedTouches) {\n"
		"\t\t\tthis.dragStart = {\n"
		"\t\t\t\tx: event.changedTouches[0].clientX,\n"
		"\t\t\t\ty: event.changedTouches[0].clientY,\n"
		"\t\t\t};\n"
		"\t\t\treturn;\n"
		"\t\t}\n"
		"\t\tthis.dragStart = {\n"
		"\t\t\tx: event.clientX,\n"
		"\t\t\ty: event.clientY,\n"
		"\t\t};\n"
		"\t};\n"
		"\n"
		"\tJOYSTICK.prototype.handleMouseMove = function( event ) {\n"
		"\t\tif ( this.dragStart === null ) return;\n"
		"\t\tevent.preventDefault();\n"
		"\t\tif (event.changedTouches) {\n"
		"\t\t\tevent.clientX = event.changedTouches[0].clientX;\n"
		"\t\t\tevent.clientY = event.changedTouches[0].clientY;\n"
		"\t\t}\n"
		"\t\tconst xDiff = event.clientX - this.dragStart.x;\n"
		"\t\tconst yDiff = event.clientY - this.dragStart.y;\n"
		"\t\tconst angle = Math.atan2( yDiff, xDiff );\n"
		"\t\tconst distance = Math.min( this.maxDiff, Math.hypot( xDiff, yDiff ) );\n"
		"\t\tconst distanceOld = Math.sqrt( Math.pow( xDiff, 2 ) + Math.pow( yDiff, 2 ) );\n"
		"\n"
		"\t\tconst xNew = distance * Math.cos( angle );\n"
		"\t\tconst yNew = distance * Math.sin( angle );\n"
		"\t\tthis.stick.style.transform = `translate3d(${xNew}px, ${yNew}px, 0px)`;\n"
		"\t\tthis.currentPos = { x: xNew, y: yNew };\n"
		"\t\tmoveBot(this.currentPos);\n"
		"\t};\n"
		"\n"
		"\tJOYSTICK.prototype.handleMouseUp = function(event) {\n"
		"\t\tif ( this.dragStart === null ) return;\n"
		"\t\tthis.stick.style.transition = '.2s';\n"
		"\t\tthis.stick.style.transform = `translate3d(0px, 0px, 0px)`;\n"
		"\t\tthis.dragStart = null;\n"
		"\t\tthis.currentPos = { x: 0, y: 0 };\n"
		"\t\tmoveBot(this.currentPos);\n"
		"\t};\n"
		"\n"
		"\tfunction moveBot(pos) {\n"
		"\t\tif(pos.x === 0 && pos.y === 0) { this.direction='stop';}\n"
		"\t\telse if(Math.abs(pos.x) > Math.abs(pos.y) ) {\n"
		"\t\t\tif(pos.x > 0) { this.direction = 'right'; }\n"
		"\t\t\telse { this.direction = 'left'; }\n"
		"\t\t}\n"
		"\t\telse{\n"
		"\t\t\tif(pos.y > 0) { this.direction = 'backward'; }\n"
		"\t\t\telse { this.direction = 'forward'; }\n"
		"\t\t}\n"
		"\t\txhttp = new XMLHttpRequest();\n"
		"\t\txhttp.onreadystatechange = function() {\n"
		"\t\t\tif(this.readyState == 4 && this.status == 200) {\n"
		"\t\t\t\tconsole.log(this.responseText);\n"
		"\t\t\t}\n"
		"\t\t};\n"
		"\t\txhttp.open('GET', this.direction, true);\n"
		"\t\txhttp.send();\n"
		"\t}\n"
		"\n"
		"\tconst joystick = new JOYSTICK (document.getElementById('joystick-base') );\n"
		"\t</script>\n"
		"\t</body>\n"
		"</html>"
	);
}

void motorStop() {
	digitalWrite(leftM1, LOW);
	digitalWrite(leftM2, LOW);
	digitalWrite(rightM1, LOW);
	digitalWrite(rightM2, LOW);
}

void handleStop() {
	motorStop();
	server.send(200, "text/html", "stopping");
}

void handleForward() {
	motorStop();
	digitalWrite(leftM1, HIGH);
	digitalWrite(leftM2, LOW);
	digitalWrite(rightM1, HIGH);
	digitalWrite(rightM2, LOW);
	server.send(200, "text/html", "Going forward");
}

void handleLeft() {
	motorStop();
	digitalWrite(rightM1, HIGH);
	digitalWrite(rightM2, LOW);
	server.send(200, "text/html", "Going left");
}

void handleRight() {
	motorStop();
	digitalWrite(leftM1, HIGH);
	digitalWrite(leftM2, LOW);
	server.send(200, "text/html", "Going right");
}

void handleBackward() {
	motorStop();
	digitalWrite(leftM1, LOW);
	digitalWrite(leftM2, HIGH);
	digitalWrite(rightM1, LOW);
	digitalWrite(rightM2, HIGH);
	server.send(200, "text/html", "Going backward");
}

void setup() {
	delay(1000);

	pinMode(leftM1, OUTPUT);
	pinMode(leftM2, OUTPUT);
	pinMode(rightM1, OUTPUT);
	pinMode(rightM2, OUTPUT);

	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid/*, password*/);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
	server.on("/forward", handleForward);
	server.on("/left", handleLeft);
	server.on("/right", handleRight);
	server.on("/backward", handleBackward);
	server.on("/stop", handleStop);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
