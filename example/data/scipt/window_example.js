
var window = function() {
  this.type = TYPE_WINDOW,
  this.title = "example window 1",
  this.position = {
    x: 10,
    y: 10
  },
  this.size = {
    x: 300,
    y: 200
  },
  this.color = {
    r: 255,
    g: 100,
    b: 50,
    a: 0
  },
  this.children = {
    button: function() {
      this.type = TYPE_BUTTON,
      this.label = "click me",
      this.position = {
        x: 10,
        y: 20
      },
      this.size = {
        x: 50,
        y: 50
      },
      this.color = {
        r: 100,
        g: 255,
        b: 0,
        a: 0
      },
      this.on_focus: null,
      this.on_hover: null,
      this.on_click: function() {
        
      }
    }
  },
}

