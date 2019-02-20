---
{{ $fields := split .Name "_" }}
{{- $name := delimit (after 1 $fields) " " -}}
title: "{{ $name | title }}"
date: {{ .Date }}
tags:
- event
- workshop
draft: true

# image: "your-talk-image.png"
description:  "Some description, used in talk summary"
where: "<a place>"
who: "<some speaker>"
{{ $date := index $fields 0 }}
{{- $year := substr $date 0 4 -}}
{{- $month := substr $date 4 2 -}}
{{- $day := substr $date 6 2 -}}
when: "{{$year}}-{{$month}}-{{$day}}"
---


