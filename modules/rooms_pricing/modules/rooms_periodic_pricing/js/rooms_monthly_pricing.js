(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {

    unit_id = Drupal.settings.roomsPricing.roomID;

    // Current month is whatever comes through -1 since js counts months starting
    // from 0
    currentMonth = Drupal.settings.roomsCalendar.currentMonth - 1;
    currentYear = Drupal.settings.roomsCalendar.currentYear;
    firstDay = Drupal.settings.roomsCalendar.firstDay;

    // The first month on the calendar
    month1 = currentMonth;
    year1 = currentYear;

    events = [];
    var url = Drupal.settings.basePath + '?q=bam/v1/pricing&units=' + unit_id + '&start_date=' + year1 + '-' + (month1+1) + '-01&duration=3M';
    $.ajax({
      url: url,
      success: function(data) {
        events = data['events'];

        $('#calendar').fullCalendar('refetchEvents');
      }
    });


    phpmonth = month1+1;
    $('#calendar').once().fullCalendar({
      schedulerLicenseKey: 'CC-Attribution-NonCommercial-NoDerivatives',
      contentHeight: 63,
      views: {
        timeline12Month: {
          type: 'timeline',
          slotDuration: { months: 1 },
          duration: { months: 12 }
        }
      },
      defaultView: 'timeline12Month',
      dayNamesShort:[Drupal.t("Sun"), Drupal.t("Mon"), Drupal.t("Tue"), Drupal.t("Wed"), Drupal.t("Thu"), Drupal.t("Fri"), Drupal.t("Sat")],
      monthNames:[Drupal.t("January"), Drupal.t("February"), Drupal.t("March"), Drupal.t("April"), Drupal.t("May"), Drupal.t("June"), Drupal.t("July"), Drupal.t("August"), Drupal.t("September"), Drupal.t("October"), Drupal.t("November"), Drupal.t("December")],
      firstDay: firstDay,
      header:{
        left: 'today prev,next',
        center: 'title',
        right: ''
      },
      defaultDate: moment([year1,phpmonth-1]),
      events: [
        { id: '1', resourceId: 'a', start: '2015-01-01', end: '2015-01-01', color: 'blue', title: '110' },
        { id: '2', resourceId: 'a', start: '2015-02-01', end: '2015-02-01', color: 'red', title: '100' },
        { id: '3', resourceId: 'a', start: '2015-03-01', end: '2015-03-01', color: 'red', title: '101' },
        { id: '4', resourceId: 'a', start: '2015-04-01', end: '2015-04-01', color: 'blue', title: '102' },
        { id: '5', resourceId: 'a', start: '2015-05-01', end: '2015-05-01', color: 'red', title: '103' },
        { id: '6', resourceId: 'a', start: '2015-06-01', end: '2015-06-01', color: 'red', title: '104' },
        { id: '7', resourceId: 'a', start: '2015-07-01', end: '2015-07-01', color: 'red', title: '105' },
        { id: '8', resourceId: 'a', start: '2015-08-01', end: '2015-08-01', color: 'blue', title: '106' },
        { id: '9', resourceId: 'a', start: '2015-09-01', end: '2015-09-01', color: 'red', title: '107' },
        { id: '10', resourceId: 'a', start: '2015-10-01', end: '2015-10-01', color: 'blue', title: '108' },
        { id: '11', resourceId: 'a', start: '2015-11-01', end: '2015-11-01', color: 'red', title: '109' },
        { id: '11', resourceId: 'a', start: '2015-12-01', end: '2015-12-01', color: 'red', title: '111' }
      ]
    });


    // Resize takes care of some quirks on occasion
    $(window).resize();
  }
};
})(jQuery);
