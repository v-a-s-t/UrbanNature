$(document).ready(function() {
  $('.aio_feed').hide();
  $('#ssid').attr('disabled', true);
  $('#pass').attr('disabled', true);

  populateForm();
  populateNetworks();
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

function populateNetworks() {
  let networks = $('#ssid');
  $.getJSON('/scan', function (json) {
      $.each(json, function (key, entry) {
          let network = $('<option></option>');

          network.attr('value', entry.SSID).text(entry.SSID);
          networks.append(network);
      });

      $('#ssid').attr('disabled', false);
      $('#pass').attr('disabled', false);
  });
}

function populateForm() {
  $.getJSON('/settings', function(json) {
    console.log(json);
    $('#user').val(json.user);
    $('#aio_key').val(json.aio_key);
    $('#startMinute').val(json.startMinute);
    $('#lat').val(json.lat);
    $('#lon').val(json.lon);

    $('#interval').children().each(function() {
      console.log("Interval: " + $(this).val());
      if (parseInt($(this).val()) === json.interval) {
        console.log("Device interval: " + $(this).val());
        $('#interval').val($(this).val());
      }
    });

    if (json.sensorFeeds) {
      $('.form-check-input').each(function() {
        let feedId = $(this).attr('id');
        if (feedId in json.sensorFeeds) {
          // sensor exists in feeds. Let's tick the box and get the feed name.
          this.checked = true;
          $('#' + feedId + '_feed').val(json.sensorFeeds[feedId]);
          $('#' + feedId + '_feed').parent().show();
        }
      });
    }

  });
}

function onSubmit(event) {
  event.preventDefault();
  $('#alert-text').hide();
  $('#alert-text').empty();

  let sensors = {};

  let formValid = true;
  let numOfSensors = 0;
  $('.form-check-input').each(function(index) {
    if (this.checked) {
      let feedId = $(this).attr('id');
      let feedName = $('#' + feedId + '_feed').val();
      console.log("Feed name: " + feedName);
      if (feedName) {
        sensors[feedId] = feedName;
        numOfSensors++;
      } else {
        console.log("Empty feed name!");
        $('#alert-text').show();
        $('#alert-text').addClass('alert-danger');
        $('#alert-text').append('Some of your selected sensors are missing an Adafruit IO feed. Please make sure you provide a feed for each sensor!<br/>');
        window.scrollTo(0, 0);
        formValid = false;
      }
    }
  });

  if (numOfSensors === 0) {
    formValid = false;
    $('#alert-text').show();
    $('#alert-text').addClass('alert-danger');
    $('#alert-text').append('Please select at least one sensor.<br/>');
    window.scrollTo(0, 0);
  }

  let data = {
    ssid: $('#ssid').children('option:selected').val(),
    pass: $('#pass').val(),
    user: $('#user').val(),
    aio_key: $('#aio_key').val(),
    interval: parseInt($('#interval').val()),
    startMinute: $('#startMinute').val(),
    lat: $('#lat').val(),
    lon: $('#lon').val(),
    sensorFeeds: sensors,
  };

  if (data.ssid === "Do Not Use WiFi") {
    data.ssid = "";
  }

  if (parseInt(data.startMinute) > 59 || parseInt(data.startMinute) < 0) {
    formValid = false;
    $('#alert-text').show();
    $('#alert-text').addClass('alert-danger');
    $('#alert-text').append('Please make sure your data collection start time is between 0 and 59 minutes.<br/>');
    window.scrollTo(0, 0);
  }

  if (!data.user || !data.aio_key) {
    formValid = false;
    $('#alert-text').show();
    $('#alert-text').addClass('alert-danger');
    $('#alert-text').append('Please provide an Adafruit IO username and key.<br/>');
    window.scrollTo(0, 0);
  }

  if (formValid) {
    console.log(data);
    $.ajax({
      type: "POST",
      url: "/settings",
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
