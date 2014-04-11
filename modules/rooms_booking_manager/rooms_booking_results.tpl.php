<?php if (isset($change_search)): ?>
  <?php print render($change_search); ?>
<?php endif; ?>

<?php if (!$booking_results): ?>
  <?php print render($no_results); ?>
  <?php print render($booking_search_form); ?>
<?php endif; ?>

<?php if (isset($style) && ($style == ROOMS_INDIVIDUAL)): ?>
  <?php if ($booking_results): ?>
    <?php print render($legend); ?>
    <?php print render($change_search); ?>

    <?php foreach ($units_per_type as $type_name => $units_per_price_level): ?>
      <div class="rooms-search-result__unit-type">

        <?php print render(${$type_name}); ?>

        <?php foreach ($units_per_price_level as $price => $units) : ?>
          <?php foreach ($units as $unit_id => $unit) : ?>
            <div class="rooms-search-result__unit-embedded" id="unit_<?php print $unit_id ?>">
            <?php
              print render($unit['unit']);
              print render($unit['price']);
              print render($unit['book_unit_form']);
            ?>
            </div>
          <?php endforeach; ?>
        <?php endforeach; ?>
      </div>
    <?php endforeach; ?>
  <?php endif; ?>
<?php endif; ?>

<?php if (isset($style) && ($style == ROOMS_PER_TYPE)): ?>
  <?php if ($booking_results): ?>
        <?php print render($units_per_type_form); ?>
  <?php endif; ?>
<?php endif; ?>
