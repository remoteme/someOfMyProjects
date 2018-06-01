var remoteme;

function setup(){

	


	remoteme = new RemoteMe({
		automaticlyConnectWS: true,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: webSocketConnectionChange,
		webRTCConnectionChange: undefined,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});
	createChart();
}


function readDataNow(){
	remoteme.sendUserSyncMessageWebSocket(temperatureArduinoDeviceId,[],onResponse);
}


function onResponse(output){
	var data = new RemoteMeData(output);


	var temp = data.popFloat32();

	var pressure = data.popFloat32();
	var humm = data.popFloat32();
	$("#tempOut").html(temp.toFixed(2)+" C");
	$("#pressOut").html((pressure/100).toFixed(2)+" hPa");
	$("#hummOut").html(humm.toFixed(2)+" %");


}

function webSocketConnectionChange(state){

	if (state==WebsocketConnectingStatusEnum.CONNECTED){
		readDataNow();
	}
}



function createChart(){

   var yestarday=moment().subtract(1, 'days').format("DD.MM.YYYY HH:mm");
   var now=moment().format("DD.MM.YYYY HH:mm");


	var url =`/api/rest/v1/data/get/dd.MM.yyyy HH:mm/${yestarday}/${now}/1,2,3/`;

	$.get(url, function(data, status){




		var trace1 = {
			x: data[0].datas,
			y:  data[0].value,
			name: "temperature",
			type: "area"
		};
		var trace2 = {
			x: data[1].datas,
			y:  data[1].value,
			name: "pressure",
			yaxis: "y2",
			type: "scatter"
		};
		var trace3 = {
			x: data[2].datas,
			y:  data[2].value,
			name: "hummanity",
			yaxis: "y3",
			type: "scatter"
		};

		var data = [trace1, trace2, trace3];
		var layout = {
			title: "multiple y-axes example",
			xaxis:{
				categoryorder: "category ascending"
			},
			yaxis: {

				title: "C",
				titlefont: {color: "#1f77b4"},
				tickfont: {color: "#1f77b4"}
			},
			yaxis2: {

				title: "PSI",
				titlefont: {color: "#ff7f0e"},
				tickfont: {color: "#ff7f0e"},
				anchor: "free",
				overlaying: "y",
				side: "left",
				position: 0.05
			},
			yaxis3: {

				title: "%Humm",
				titlefont: {color: "#d62728"},
				tickfont: {color: "#d62728"},
				anchor: "x",
				overlaying: "y",
				side: "right"
			}
		};

		Plotly.newPlot('chartDiv', data,layout);

	});

}