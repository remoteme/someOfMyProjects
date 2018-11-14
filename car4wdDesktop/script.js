



var ot;
var speedmeter;
var turnMeter;
var speedControll;
var turn;
var carController;
var leftSideSpeedMeter ;
var rightSideSpeedMeter ;
var cameraPosition;




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

	leftSideSpeedMeter = new SpeedMeter('leftSideSpeed',"left",150,150);
	rightSideSpeedMeter = new SpeedMeter('rightSideSpeed',"right",150,150);


	speedControll=new Control(-255,255,3,function(value){
		carController.setSpeed(value);
		speedmeter.setValue(value*255);
		send();
	});


	turn=new Control(-255,255,3,function(value){
		carController.setTurn(value);
		turnMeter.setValue(value*255);
		send();
	});
	turn.idleWait=0;
	turn.freeAccelerate=40;
	turn.accelerate=40;


	carController = new CarController4WD();
	carController.xAxeCenter= 560;
	carController.xAxeRange=200;
	carController.yAxeCenter=430;
	carController.yAxeRange=200;

	carController.setSpeed(0);
	carController.setTurn(0);
	carController.setCameraPosition(0.5,0.5);


	setupKeyboard();


	$("#remoteVideo").mousemove( function( event ) {
		var video = $("#remoteVideo");
		var parentOffset =video.offset();

		var pox_x = event.pageX/video.width();
		var pos_y = event.pageY/video.height();

		carController.setCameraPosition(pox_x*2-1,pos_y*2-1);
		cameraPosition.setPosition(pox_x,pos_y);
		send();
	});



	$('#cameraCenter').on('click', function() {
		carController.setCameraPosition(0,0);
		cameraPosition.setPosition(0.5,0.5);
		send();

	});
	//-----------

	$('#messageMode button').on('click', function() {
		$('#messageMode button').removeClass('active')
		$(this).addClass('active');


	});


	$('#messageMode button').on('click', function() {
		$('#messageMode button').removeClass('active')
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






function send(){
	if  (remoteme.isWebRTCConnected()){
		ot.defaultDelay=200;
	}else{
		ot.defaultDelay=400;
	}
	ot.execute(sendNow)
}



function sendNow(){
	carController.compute();


	leftSideSpeedMeter.setValue(carController.getLeftSideSpeed()*255 );
	rightSideSpeedMeter.setValue(carController.getRightSideSpeed()*255 );

	var ret = new RemoteMeData(9);

	ret.putByte(1);//mode 1 send camera and motors
	ret.putShort(carController.getCameraX() );
	ret.putShort(carController.getCameraY() );

	ret.putByte(carController.getMotorMode(carController.getRightSideSpeed()) );
	ret.putByte(Math.abs(carController.getRightSideSpeed()*255 ));

	ret.putByte(carController.getMotorMode(-carController.getLeftSideSpeed()) );
	ret.putByte(Math.abs(carController.getLeftSideSpeed()*255) );



	remoteme.sendUserMessageWebrtc(carScriptDeviceId,ret);




}



function webSocketConnectionChange(state){
	console.info("webosvcket change "+state);
	if (state==ConnectingStatusEnum.CONNECTED) {
		$("#webSocketState").removeClass('btn-secondary');
		$("#webSocketState").addClass('btn-success');
	}else if (state==ConnectingStatusEnum.DISCONNECTED || state==ConnectingStatusEnum.ERROR){
		$("#webSocketState").removeClass('btn-success');
		$("#webSocketState").addClass('btn-secondary');
	}

}

function webRtcConnectionChange(state){
	if (state==ConnectingStatusEnum.CONNECTED){
		$("#webRTCState").removeClass('btn-secondary');
		$("#webRTCState").addClass('btn-success');


	}else{
		$("#webRTCState").removeClass('btn-success');
		$("#webRTCState").addClass('btn-secondary');
	}

	if (state==ConnectingStatusEnum.CONNECTED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Connected");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}else if (state==ConnectingStatusEnum.CONNECTING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Connecting");
	}else if (state==ConnectingStatusEnum.DISCONNECTING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Disconnecting");
	}else if (state==ConnectingStatusEnum.CHECKING) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Checking");
	}else if (state==ConnectingStatusEnum.DISCONNECTED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Disconnected");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}else if (state==ConnectingStatusEnum.FAILED) {
		$("#webRTCStatusWindow").modal("show");
		$("#webRTCStatusWindow").find(".statusText").html("View Failed");
		setTimeout(function(){
			$("#webRTCStatusWindow").modal("hide");
		},1500);
	}
}



