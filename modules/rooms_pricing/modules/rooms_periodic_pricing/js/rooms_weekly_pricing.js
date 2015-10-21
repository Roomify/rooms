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
    var url = Drupal.settings.basePath + '?q=bam/v1/pricing&units=' + unit_id + '&start_date=' + year1 + '-' + (month1+1) + '-01&duration=1Y';
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
      contentHeight: 90,
      views: {
        timeline8Week: {
          type: 'timeline',
          slotDuration: { days: 7 },
          duration: { days: 64 }
        }
      },
      defaultView: 'timeline8Week',
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
        { id: '1', resourceId: 'a', start: '2015-09-27', end: '2015-10-03', color: 'blue', title: '110' },
        { id: '2', resourceId: 'a', start: '2015-10-04', end: '2015-10-10', color: 'red', title: '100' },
        { id: '3', resourceId: 'a', start: '2015-10-11', end: '2015-10-17', color: 'red', title: '101' },
        { id: '4', resourceId: 'a', start: '2015-10-18', end: '2015-10-24', color: 'blue', title: '102' },
        { id: '5', resourceId: 'a', start: '2015-10-25', end: '2015-10-31', color: 'red', title: '103' },
        { id: '6', resourceId: 'a', start: '2015-11-01', end: '2015-11-07', color: 'red', title: '104' },
        { id: '7', resourceId: 'a', start: '2015-11-08', end: '2015-11-14', color: 'red', title: '105' },
        { id: '8', resourceId: 'a', start: '2015-11-15', end: '2015-11-21', color: 'blue', title: '106' },
        { id: '9', resourceId: 'a', start: '2015-11-22', end: '2015-11-28', color: 'red', title: '107' },
        { id: '10', resourceId: 'a', start: '2015-11-29', end: '2015-12-05', color: 'blue', title: '108' },
        { id: '11', resourceId: 'a', start: '2015-12-06', end: '2015-11-12', color: 'red', title: '109' },
        { id: '11', resourceId: 'a', start: '2015-12-13', end: '2015-11-19', color: 'red', title: '111' }
      ]
    });


    // Resize takes care of some quirks on occasion
    $(window).resize();
  }
};
})(jQuery);
