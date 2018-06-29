



var ot;
var speedmeter;
var turnMeter;
var speedControll;
var turn;
var cameraPosition;


var carData={
	"speed":0,
	"turn":0,
}
var cameraData={
	"posX":0,
	"posY":0,
}

xAxeCenter= 600;
xAxeRange=200;
yAxeCenter=520;
yAxeRange=200;


function setupKeyboard(){
	$(document).keydown(function(e){

		if (e.which==38){//arrow up
			speedControll.setMode(1,e.shiftKey);
		}else if (e.which==40){//arrow down
			speedControll.setMode(-1,e.shiftKey);
		}else if (e.which==39){//arrow right
			turn.setMode(1);
		}else if (e.which==37){//arrow left
			turn.setMode(-1);
		}
	});

	$(document).keyup(function(e){
		if (e.which==38){//arrow up
			speedControll.setMode(0);
		}else if (e.which==40){//arrow down
			speedControll.setMode(0);
		}else if (e.which==39){//arrow right
			turn.setMode(0);
		}else if (e.which==37){//arrow left
			turn.setMode(0);
		}

	});
}


var remoteme;

function setupComponents(){
	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:true,
		webSocketConnectionChange: webSocketConnectionChange,
		webRTCConnectionChange: webRtcConnectionChange,
	});

	ot=new OperationTimer(200);
	cameraPosition=new DotPosition("cameraPosition",100,100,-100,100,-100,100);



	speedmeter = new SpeedMeter('speed',"Speed",300,300);
	turnMeter = new TurnMeter('turn',320,100);



	speedControll=new Control(-255,255,5,function(value){
		speedmeter.setValue(value*255);
		carData.speed=Math.round(value*255);
		sendMotor();
	});
	speedControll.idleWait=0;
	speedControll.freeAccelerate=400;
	speedControll.accelerate=400;

	turn=new Control(-255,255,2,function(value){
		carData.turn=Math.round(value*255);
		turnMeter.setValue(value*255);
		sendMotor();
	});
	turn.idleWait=0;
	turn.freeAccelerate=400;
	turn.accelerate=400;



	setupKeyboard();


	$("#remoteVideo").mousemove( function( event ) {
		var video = $("#remoteVideo");
		var parentOffset =video.offset();

		var pos_x = event.pageX/video.width();
		var pos_y = event.pageY/video.height();


		cameraData.posX=(pos_x-0.5)*2;
		cameraData.posY=(pos_y-0.5)*2;

		cameraPosition.setPosition(pos_x,pos_y);
		sendCamera();
	});



	$('#cameraCenter').on('click', function() {
		cameraPosition.setPosition(0.5,0.5);
		cameraData.posX=0;
		cameraData.posY=0;
		sendCamera();

	});
	//-----------

	$('#messageMode button').on('click', function() {
		$('#messageMode button').removeClass('active');
		$(this).addClass('active');


	});


	$('#messageMode button').on('click', function() {
		$('#messageMode button').removeClass('active');
		$(this).addClass('active');
	});




	$('#webSocketState').on('click', function() {
		if (remoteme.isWebSocketConnected()){
			remoteme.disconnectWebSocket();
		}else{
			remoteme.connectWebSocket();
		}

	});
	$('#webRTCState').on('click', function() {
		if (remoteme.isWebRTCConnected()){
			remoteme.disconnectWebRTC();
		}else{
			remoteme.connectWebRTC();
		}
	});




}
counter=0;






function sendMotor(){
	if  (remoteme.isWebRTCConnected()){
		ot.defaultDelay=200;
	}else{
		ot.defaultDelay=300;
	}
	ot.execute(sendMotorNow)
}

function sendCamera(){
	if  (remoteme.isWebRTCConnected()){
		ot.defaultDelay=150;
	}else{
		ot.defaultDelay=300;
	}
	ot.execute(sendCameraNow)
}


function getMode(speed){
	if (speed==0){
		return 1;
	}else{
		return speed>0?2:3;
	}
}
function sendMotorNow(){

	console.info(carData);
	console.info(cameraPosition.pos_x);


	console.info("sending ");

	var ret = new RemoteMeData(7);

	ret.putByte(1);//mode 1  motors

	ret.putShort(getMode(carData.speed));
	ret.putShort(Math.abs(carData.speed));
						//600
	ret.putShort( carData.turn/255.0*200+400);//400 center
						//200 turn left


	remoteme.sendUserMessageWebrtc(carScriptDeviceId,ret);


	if (cameraData.posX!=0 && cameraData.posY!=0){
		cameraData.posX=0;
		cameraData.posY=0;
		sendCameraNow();
	}


}

function sendCameraNow(){


	var ret = new RemoteMeData(7);

	ret.putByte(2);//mode 1 send camera and motors
	ret.putShort(Math.round(-cameraData.posX*xAxeRange+xAxeCenter) );
	ret.putShort(Math.round(cameraData.posY*yAxeRange+yAxeCenter));

	console.info(cameraData.posX*xAxeRange+xAxeCenter);
	console.info(cameraData.posY*yAxeRange+yAxeCenter);

	remoteme.sendUserMessageWebrtc(carScriptDeviceId,ret);




}


function webSocketConnectionChange(state){
	console.info("webosvcket change "+state);
	if (state==WebsocketConnectingStatusEnum.CONNECTED) {
		$("#webSocketState").removeClass('btn-secondary');
		$("#webSocketState").addClass('btn-success');
	}else if (state==WebsocketConnectingStatusEnum.DISCONNECTED || state==WebsocketConnectingStatusEnum.ERROR){
		$("#webSocketState").removeClass('btn-success');
		$("#webSocketState").addClass('btn-secondary');
	}

}

function webRtcConnectionChange(state){
	if (state==WebrtcConnectingStatusEnum.CONNECTED){
		$("#webRTCState").removeClass('btn-secondary');
		$("#webRTCState").addClass('btn-success');


	}else{
		$("#webRTCState").removeClass('btn-success');
		$("#webRTCState").addClass('btn-secondary');
	}

	if (state==WebrtcConnectingStatusEnum.CONNECTED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Connected");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}else if (state==WebrtcConnectingStatusEnum.CONNECTING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Connecting");
	}else if (state==WebrtcConnectingStatusEnum.DISCONNECTING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Disconnecting");
	}else if (state==WebrtcConnectingStatusEnum.CHECKING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Checking");
	}else if (state==WebrtcConnectingStatusEnum.DISCONNECTED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Disconnected");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}else if (state==WebrtcConnectingStatusEnum.FAILED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Failed");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}
}



