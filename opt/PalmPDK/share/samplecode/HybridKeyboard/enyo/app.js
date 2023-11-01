/* HybridKeyboard app.js */

enyo.kind({
	name: "HybridKeyboardApp",
	kind: "VFlexBox",
	components: [
		{kind: enyo.Hybrid, name: "plugin", executable: "hybridkeyboard_plugin", width: 640, height: 480, 
		style: "padding: 20px", takeKeyboardFocus: false, onclick: "focusPlugin"}
	],
	
	create: function() {
		this.inherited(arguments);
	},
	
	focusPlugin: function() {
		this.$.plugin.focus();
	}
});
