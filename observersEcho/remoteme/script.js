var remoteme;

function setup() {
	remoteme = new RemoteMe();

	remoteme.getObservers().observeIntegerBoolean("pam", function (value, val2) {
		console.info(value + " " + val2);
	});

	remoteme.getObservers().observeText("pam2", function (value) {
		console.info(value);
	});

	remoteme.getObservers().observeDouble("pam2", function (value) {
		console.info(value);
	});

	remoteme.getObservers().observeSmallInteger2("pam", function (value, val2) {
		console.info(value + " " + val2);
	});

	remoteme.getObservers().observeSmallInteger3("pam", function (value, val2, val3) {
		console.info(value + " " + val2 + " " + val3);
	});


	$('.b1').on('click', function () {
		remoteme.getObservers().setIntegerBoolean("pam", 1235, true);
	});

	$('.b2').on('click', function () {
		remoteme.getObservers().setText("pam2", "maciek2")
	});

	$('.b3').on('click', function () {
		remoteme.getObservers().setDouble("pam2", 123.456);
	});
	$('.b4').on('click', function () {
		remoteme.getObservers().setSmallInteger2("pam", 123, 456);
	});
	$('.b5').on('click', function () {
		remoteme.getObservers().setSmallInteger3("pam", 123, 45, -67);
	});


}

