$(document).ready(function() {
  $('.aio_feed').hide();

  // TODO load current settings by requesting from ESP32
  configureEvents();
});

function configureEvents() {
  $('.form-check-input').change(function() {
    let myId = $(this).attr('id');
    let feedId = '#' + myId + '_feed';
    if (this.checked) {
      $(feedId).parent().show();
    } else {
      $(feedId).parent().hide();
    }
  });

  $('#btn-submit').click(onSubmit);
}

function populateForm(json) {

}

function onSubmit(event) {
  event.preventDefault();

  let sensors = [];

  let formValid = true;

  $('.form-check-input').each(function(index) {
    if (this.checked) {
      let feedId = $(this).attr('id');
      let feedName = $('#' + feedId + '_feed').val();
      console.log("Feed name: " + feedName);
      if (feedName) {
        sensors.push({
          feedId: feedId,
          feedName: feedName
        });
      } else {
        console.log("Empty feed name!");
        $('#alert-text').show();
        $('#alert-text').addClass('alert-danger');
        $('#alert-text').text('Some of your selected sensors are missing an Adafruit IO feed. Please make sure you provide a feed for each sensor!');
        window.scrollTo(0, 0);
        formValid = false;
      }
    }
  });

  let data = {
    wifi_ssid: $('#wifi_ssid').children('option:selected').val(),
    wifi_pass: $('#wifi_pass').val(),
    sensors: sensors,
    interval: $('#interval').children('option:selected').attr('interval')
  };

  if (formValid) {
    $.ajax({
      type: "POST",
      url: "/credentials",
      data: JSON.stringify(data),
      dataType: 'text',
      contentType: 'application/json; charset=utf-8',
      cache: false,
      timeout: 15000,
      async: false,
      success: function(response, textStatus, jqXHR) {
        console.log(response);
        $('#alert-text').show();
        $('#alert-text').removeClass('alert-danger');
        $('#alert-text').addClass('alert-success');
        $('#alert-text').text('Settings saved. Your device will restart now.');
        window.scrollTo(0, 0);
      },
      error: function (jqXHR, textStatus, errorThrown) {
        console.log(jqXHR);
        console.log(textStatus);
        console.log(errorThrown);
        $('#alert-text').show();
        $('#alert-text').addClass('alert-danger');
        $('#alert-text').text('Error: Couldn\'t save settings!');
        window.scrollTo(0, 0);
      }
      });
    }
}
