var remoteme;

function setup(){

	


	remoteme = new RemoteMe({
		automaticlyConnectWS: false,
		automaticlyConnectWebRTC:false,
		webSocketConnectionChange: undefined,
		webRTCConnectionChange: undefined,
		mediaConstraints: {'mandatory': {'OfferToReceiveAudio': false, 'OfferToReceiveVideo': false}}
	});
	createChart();
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
			name: "humidity",
			yaxis: "y3",
			type: "scatter"
		};

		var data = [trace1, trace2, trace3];
		var layout = {
			title: "temperature humidity pressure",
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