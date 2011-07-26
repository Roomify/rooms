<?php dpm($variables) ?>
<?php if (!$booking_results): ?>
  <?php print render($booking_search_form); ?>
  <?php print render($no_results); ?>
<?php endif; ?>

<?php if (isset($style) && ($style == 'individual')): ?>
  <?php if ($booking_results): ?>
    <?php foreach ($units_per_type as $unit_type => $units_per_price_level) {
      print $unit_type;
      foreach ($units_per_price_level as $price => $unit) {
        print render($unit[key($unit)]['unit']);
        print render($unit[key($unit)]['price']);
        print render($unit[key($unit)]['book_unit_form']);
      }
    }?>
  <?php endif; ?>
<?php endif; ?>

<?php if (isset($style) && ($style == 'per_type')): ?>
  <?php if ($booking_results): ?>
        <?php print render($units_per_type_form); ?>
  <?php endif; ?>
<?php endif; ?>
  
  
