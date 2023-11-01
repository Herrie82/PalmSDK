/* FileTree app.js */

/* Here, we create a specialization of enyo.Hybrid for our FileTree plugin.  This is done to simplify the API for
 * the user. */

enyo.kind({
	name: "FileTreePlugin",
	kind: "enyo.Hybrid",
	width: 0,
	height: 0,
	executable: "filetree_plugin",

	create: function() {
		this.inherited(arguments);
		// we create this as a deferred callback so we can call back into the
		// plugin immediately
		this.addCallback("getFilesResult", enyo.bind(this, this._getFilesResultsCallback), true);
	},
	
	_resultsCallbacks: [],
	_getFilesResultsCallback: function(filesJSON) {
		console.error("***** FileTreePlugin: _getFilesResultsCallback");
		// we rely on the fact that calls to the plugin will result in callbacks happening
		// in the order that the calls were made to do a first-in, first-out queue
		var callback = this._resultsCallbacks.shift();
		if (callback) {
			callback(enyo.json.parse(filesJSON));
		}
		else {
			console.error("FileTreePlugin: got results with no callbacks registered: " + filesJSON);
		}
	},
	
	getFiles: function(directory, pattern, callback) {
		if (window.PalmSystem) {
			console.error("***** FileTreePlugin: getFiles");
			this._resultsCallbacks.push(callback);
			this.callPluginMethodDeferred(enyo.nop, "getFiles", directory, pattern);
		}
		else {
			// if not on device, return mock data
			enyo.nextTick(this, function() { callback([
					{ type: "file", name: "doc.txt", size: 7684223, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "foobar.mp3", size: 1024000, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "README", size: 5097, date: "2009-06-06 12:00:00" },
					{ type: "directory", name: "data", date: "2009-06-06 13:15:30" },
					{ type: "directory", name: "My Documents", date: "2011-06-06 13:15:30" },
					{ type: "file", name: "doc.txt 2", size: 7684223, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "foobar.mp3 2", size: 1024000, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "README 2", size: 5097, date: "2009-06-06 12:00:00" },
					{ type: "directory", name: "data 2", date: "2009-06-06 13:15:30" },
					{ type: "directory", name: "My Documents 2", date: "2011-06-06 13:15:30" },
					{ type: "file", name: "doc.txt 3", size: 7684223, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "foobar.mp3 3", size: 1024000, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "README 3", size: 5097, date: "2009-06-06 12:00:00" },
					{ type: "directory", name: "data 3", date: "2009-06-06 13:15:30" },
					{ type: "directory", name: "My Documents 3", date: "2011-06-06 13:15:30" },
					{ type: "file", name: "doc.txt 4", size: 7684223, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "foobar.mp3 4", size: 1024000, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "README 4", size: 5097, date: "2009-06-06 12:00:00" },
					{ type: "directory", name: "data 4", date: "2009-06-06 13:15:30" },
					{ type: "directory", name: "My Documents 4", date: "2011-06-06 13:15:30" },
					{ type: "file", name: "doc.txt 5", size: 7684223, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "foobar.mp3 5", size: 1024000, date: "2009-06-06 12:00:00" },
					{ type: "file", name: "README 5", size: 5097, date: "2009-06-06 12:00:00" },
					{ type: "directory", name: "data 5", date: "2009-06-06 13:15:30" },
					{ type: "directory", name: "My Documents 5", date: "2011-06-06 13:15:30" }
				]); });
		}
	}
});

enyo.kind({
	name: "FileTreeApp",
	kind: "VFlexBox",
	components: [
		{kind: FileTreePlugin, name: "plugin"},
		{kind: "PageHeader", components: [
			{content: "Current Directory: /", name: "directory", flex: 1},
			{kind: "Button", name: "parent", caption: "Parent", disabled: true, onclick: "gotoParent"}]},
		{kind: "Scroller", flex: 1, components: [
			{name: "list", kind: "VirtualRepeater", onSetupRow: "listGetItem", onclick: "changeDirectory",
				components: [{name: "item", kind: "Item", allowHtml: true}]}
		]},
	],
	files: [],
	directory: "/",
	
	create: function() {
		this.inherited(arguments);
		this.directoryChanged();
	},
	
	directoryChanged: function() {
		this.$.directory.setContent("Current Directory: " + this.directory);
		this.$.plugin.getFiles(this.directory, "*", enyo.bind(this, this.updateFileList));
		this.$.parent.setDisabled(this.directory === "/");
	},
	
	updateFileList: function(files) {
		this.files = files;
		this.$.list.render();
		this.$.scroller.setScrollTop(0);
	},

	listGetItem: function(inSender, inIndex) {
		if (inIndex < this.files.length) {
			if (this.$.item) {
				// important to escape this, as filenames are user-generated content
				this.$.item.setContent(this.files[inIndex].name);
				if (this.files[inIndex].type === "directory") {
					this.$.item.setStyle("font-weight: bold");
				}
			}
			return true;
		}
		return false;
	},
	
	changeDirectory: function(inSender, inEvent) {
		if (this.files[inEvent.rowIndex].type === "directory") {
			if (this.directory !== "/") { this.directory += "/"; }
			this.directory += this.files[inEvent.rowIndex].name;
			this.directoryChanged();
		}
	},

	gotoParent: function() {
		if (this.directory !== "/") {
		    this.directory = this.directory.replace(/\/[^\/]*$/, "");
			if (this.directory === "") {
				this.directory = "/";
			}
			this.directoryChanged();
		}
	}
});
