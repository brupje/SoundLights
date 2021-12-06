
function updateMode(ledmode,red,green,blue,brightness,delay) {

  var data = {
    mode: ledmode,
    red: red,
    green: green,
    blue: blue,
    brightness:brightness,
    delay:delay
  };
  $.ajax({
    type: "POST",
    url: '/config/set',
    dataType:'json',
    data: JSON.stringify(data)
  });
}

function update() {
  updateMode(parseInt($('#submenu').val()),parseInt($('#red').val()),parseInt($('#green').val())
  ,parseInt($('#blue').val())
  ,parseInt($('#brightness').val())
  ,parseInt($('#delay').val())
  
  );
}

$(document).on('change','select', function() {
update();
});
