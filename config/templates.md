## ledger-entry
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

## ledger-stats
{stat-name}: {stat-value} {stat-unit}
<svg class="graph" width="99%" height="10" viewBox="0 0 100% 10">
<rect x="-1" y="0" width={stat-percent} height="10" rx="5" ry="5" fill="{stat-color}"/>
</svg>

## footer
</ol></div></div></body></html>

## header
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-9">
    <meta name="description" content="Keeper of time">
    <meta name="viewport" content="width=device-width, initial-scale=0.0, maximum-scale=1">
    <title>Timeseries</title>
    <link rel="icon" href="/links/logo.png" type="image/icon type">
    <link href="/links/style.css" rel="stylesheet" type="text/css" media="all">
</head>

<body>
    <div class="time-container">
    <div class="entries">
    <ol>

## that's all folks!!