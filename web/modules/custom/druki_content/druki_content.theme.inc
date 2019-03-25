<?php

/**
 * @file
 * Main file for custom theme hook preprocess.
 */

use Drupal\Core\Render\Element;
use Drupal\Core\Template\Attribute;
use Drupal\paragraphs\ParagraphInterface;

/**
 * Prepares variables for druki content templates.
 *
 * Default template: druki-content.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - elements: An associative array containing the druki content information
 *   and any fields attached to the entity.
 *   - attributes: HTML attributes for the containing element.
 */
function template_preprocess_druki_content(array &$variables): void {
  foreach (Element::children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  $variables['druki_content'] = $variables['elements']['#druki_content'];
  $variables['view_mode'] = $variables['elements']['#view_mode'];
}

/**
 * Implements hook_preprocess_HOOK() for druki-content-toc.html.twig.
 */
function template_preprocess_druki_content_toc(array &$variables): void {
  /** @var \Drupal\druki_content\Entity\DrukiContentInterface $druki_content */
  $druki_content = $variables['druki_content'];
  $variables['links'] = [];
  $headings = $druki_content->get('content')->filter(function ($item) {
    return $item->entity->bundle() == 'druki_heading';
  });

  if (!$headings->isEmpty()) {
    $flat_links = [];
    foreach ($headings as $item) {
      /** @var \Drupal\paragraphs\ParagraphInterface $paragraph */
      $paragraph = $item->entity;
      $flat_links[] = _druki_content_toc_prepare_link($paragraph, $flat_links);
    }

    $variables['links'] = _druki_content_toc_build_tree($flat_links);
  }
}

function _druki_content_toc_prepare_link(ParagraphInterface $paragraph, array $links = []): array {
  // Used for building tree, since we have flat tree of headings.
  $internal_id = &drupal_static(__FUNCTION__ . ':internal_id', 0);
  $internal_id++;

  $title = $paragraph->get('druki_textfield_formatted')->value;
  $heading_level = $paragraph->get('druki_heading_level')->value;

  $anchor = Drupal::transliteration()->transliterate($title);
  $anchor = strtolower($anchor);
  $anchor = trim($anchor);
  // Replace all spaces with dash.
  $anchor = preg_replace("/[\s_]/", '-', $anchor);
  // Remove everything else. Only alphabet and dash is allowed.
  $anchor = preg_replace("/[^a-z-]/", '', $anchor);

  switch ($heading_level) {
    case 'h2':
    default:
      $heading_level_int = 2;
      break;

    case 'h3':
      $heading_level_int = 3;
      break;

    case 'h4':
      $heading_level_int = 4;
      break;

    case 'h5':
      $heading_level_int = 5;
      break;

    case 'h6':
      $heading_level_int = 6;
      break;
  }

  foreach (array_reverse($links) as $link) {
    if ($link['level'] < $heading_level_int) {
      $parent_id = $link['id'];
      break;
    }
  }

  if (!isset($parent_id)) {
    $parent_id = 0;
  }

  return [
    'id' => $internal_id,
    'text' => $title,
    'anchor' => $anchor,
    'level' => $heading_level_int,
    'parent_id' => $parent_id,
  ];
}

function _druki_content_toc_build_tree(array $links, $parent_id = 0) {
  $tree = [];

  foreach ($links as $link) {
    if ($link['parent_id'] == $parent_id) {
      $children = _druki_content_toc_build_tree($links, $link['id']);

      if ($children) {
        $link['children'] = $children;
      }

      $tree[] = $link;
    }
  }

  return $tree;
}