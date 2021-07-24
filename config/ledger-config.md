## start

16/06/2021

## stats

0 | sleep | hrs | 12 | #1b4332 |
1 | mood | /10 | 10 | #2d6a4f |

## sectors

0 | website | #333 |
1 | ds | #444 |
2 | tracker | #555 |
3 | pixel-art | #666 |

## tasks

0 | planning | #6b705c |
1 | researching | #a5a58d |
2 | implementing | #b7b7a4 |
3 | testing | #ffe8d6 |
4 | documenting | #ddbea9 |
5 | maintaining | #cb997e |

## entry_template

<li>
<details id="{id}">
<summary>
{date}
</summary>
<p>
{note}
<hr>
{stats}
<hr>
<div class="pie-container">
{sectors-pie}
<div class="pie-legends">
{sectors-legend}
</div>
</div>
</p>
</details>
</li>

## stats_template

{stat-name}: {stat-value} {stat-unit}
<svg class="graph" width="100%" height="10" viewBox="0 0 100% 10">
<rect x="0" y="0" width={stat-percent} height="10" rx="5" ry="5" fill="{stat-color}"/>
</svg>

## footer
</ol></div></div></body></html>

## header

<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">
    <meta name="description" content="Keeper of time">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1">
    <title>Timeseries</title>
    <link rel="icon" href="/links/logo.png" type="image/icon type">
    <link href="/links/style.css" rel="stylesheet" type="text/css" media="all">
</head>

<body>
    <div class="time-container">
    <div class="entries">
    <ol>

## that's all folks!!
