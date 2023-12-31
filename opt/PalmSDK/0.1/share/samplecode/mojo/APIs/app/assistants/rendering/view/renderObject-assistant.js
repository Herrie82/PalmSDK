function RenderObjectAssistant() {
	/* this is the creator function for your scene assistant object. It will be passed all the 
	   additional parameters (after the scene name) that were passed to pushScene. The reference
	   to the scene controller (this.controller) has not be established yet, so any initialization
	   that needs the scene controller should be done in the setup function below. */
 }
    

RenderObjectAssistant.prototype.setup = function() {
	var myObject = {
		name: "Phil",
		address: "1205 Johnson",
		phone: "408-431-2323"
	}
	
	//render the object using a partial template.
	var content = Mojo.View.render({object: myObject,template: 'rendering/view/renderObject/item'});
	this.controller.get('items').innerHTML = (content);  
}

RenderObjectAssistant.prototype.activate = function(event) {
	/* put in event handlers here that should only be in effect when this scene is active. For
	   example, key handlers that are observing the document */
}


RenderObjectAssistant.prototype.deactivate = function(event) {
	/* remove any event handlers you added in activate and do any other cleanup that should happen before
	   this scene is popped or another scene is pushed on top */
}

RenderObjectAssistant.prototype.cleanup = function(event) {
	/* this function should do any cleanup needed before the scene is destroyed as 
	   a result of being popped off the scene stack */
}