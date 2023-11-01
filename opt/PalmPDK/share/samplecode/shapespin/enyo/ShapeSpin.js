/* ShapeSpin.js */

enyo.kind({
	name: "ShapeSpinApp",
	kind: "VFlexBox",
	published: { currentAngle: 50 },
	components: [
		{ kind: "Control", style: "padding: 0 5px;", components: [
			{ kind: "Slider", onChanging: "sliderChange", onChange: "sliderChange", minimum: 0, maximum: 100 },
			{ kind: "Slider", onChanging: "sizeSliderChange", onChange: "sizeSliderChange", minimum: 480, maximum: 800, position: 640 } ] },
		{ kind: enyo.Control, name: "output", style: "background-color: LightSkyBlue; padding: 2px 6px;" },
		{ kind: enyo.Hybrid, name: "plugin", width: 640, height: 480, params: [ "2" ],
		  executable: "shapespin_plugin", alphablend: false, bgcolor: "0000FF",
		  style: "padding: 10px;", onPluginReady: "handlePluginReady" }
	],

	pluginReady: false,
	
	create: function() {
		this.inherited(arguments);
		this.$.plugin.addCallback("testFunc", enyo.bind(this, "handleTestFunc"), true);
		this.currentAngleChanged();
	},

	currentAngleChanged: function() {
		var angle = this.currentAngle;

		// clip angle to valid values
		if (angle < 0) angle = 0;
		if (angle > 100) angle = 100;
	    this.currentAngle = angle;
		this.$.slider.setPosition(this.currentAngle);

		if (this.pluginReady) {
			try {
				var status = this.$.plugin.callPluginMethod("setAngle", angle);
				this.outputMessage("X: " + JSON.stringify(status));
			}
			catch (e) {
				this.outputMessage("Plugin exception: " + e);
			}
		}
		else {
			this.outputMessage("plugin not ready");
		}
	},

	outputMessage: function(message) {
		this.$.output.setContent(message);
		console.log("*** " + message);
	},
	
	handlePluginReady: function(inSender) {
		this.outputMessage("plugin initalized");
		this.pluginReady = true;
	},

	handleTestFunc: function(a1, a2) {
		this.outputMessage("plugin called testFunc() with arguments " + a1 + " and " + a2);
	},

	sliderChange: function(inSender) {
		this.setCurrentAngle(inSender.getPosition());
	},

	sizeSliderChange: function(inSender) {
		var newWidth = inSender.getPosition();
		var newHeight = Math.floor(newWidth * 0.75);
		this.$.plugin.setWidth(newWidth);
		this.$.plugin.setHeight(newHeight);
		this.outputMessage("resized to " + newWidth + "x" + newHeight);
	},
	
	setAngleResult: function(status) {
		this.outputMessage("X: " + JSON.stringify(status));
	},
	
	rendered: function() {
		this.inherited(arguments);
		var f = enyo.bind(this, "setAngleResult");
		// this queues up a few calls to be made when the plugin in ready
		this.$.plugin.callPluginMethodDeferred(f, "setAngle", 23);
		this.$.plugin.callPluginMethodDeferred(f, "setAngle", 42);
		this.$.plugin.callPluginMethodDeferred(f, "setAngle", 73);
	},

	gesturestartHandler: function(inSender, event) {
		console.log("***** gesturestart: event.scale: " + event.scale + ", event.centerX: " + event.centerX + ", event.centerY: " + event.centerY);
	    this.angleAtGestureStart = this.currentAngle;
	},
	gesturechangeHandler: function(inSender, event) {
		enyo.stopEvent(event);
		console.log("***** gesturechange: event.scale: " + event.scale + ", event.centerX: " + event.centerX + ", event.centerY: " + event.centerY);

		// in testing on touchpad, scale ranged in 0-1 for pinch in, 1-20 for zoom out
		var angle = this.angleAtGestureStart * event.scale;
		this.setCurrentAngle(angle);
	},
	gestureendHandler: function(inSender, event) {
		enyo.stopEvent(event);
		console.log("***** gestureend: event.scale: " + event.scale + ", event.centerX: " + event.centerX + ", event.centerY: " + event.centerY);
	}

});