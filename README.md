# Webform Calculator

Provides a `formula` webform component for computed values. You can enter a
mathematical calculation based on the other fields on the webform. For example,
you could create a formula field with the value `formkey1 * 0.1 * formkey2`.

## Features

 - Creates new component type that calculates values of other components while
   you are typing and saves the result to the database.
 - Supports component types: number, textfield, checkboxes, radios, select.
 - Available operators for formula value: `+`, `-`, `*`, `/`, `(`, `)`, `%`

## Dependencies
 - Webform

## Installation

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules
- Create a webform.
- Add webform field(s) of number, textfield, checkboxes, radios, or select type.
- Create a formula webform field and add a formula that uses the fields added
  earlier.
- View this webform and fill newly added fields. The formula field should 
  update automatically based on the field values and the formula you entered.

## Issues

Bugs and Feature requests should be reported in the 
[Issue Queue](https://github.com/backdrop-contrib/webform_calculator/issues)

## Current Maintainers

- [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org)

## Credits

- Ported to Backdrop CMS by [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org).
- Maintained for Drupal by [colemanw](https://www.drupal.org/u/colemanw) and
  [rafal.enden](https://www.drupal.org/u/rafal.enden).

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
