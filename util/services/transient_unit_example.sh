#!/bin/bash
#
# Some examples of systemd-run to setup a transient unit
#
#  --user option runs as user service, doesn't require password authentication
#
# The following command will touch a file after 30 seconds.
#
systemd-run --on-active=30 --timer-property=AccuracySec=100ms /bin/touch /tmp/foo
#
# relative time
# use '--on-active='
#    If no time unit is specified, generally seconds are assumed, but some exceptions exist and
#    are marked as such. In a few cases "ns", "nsec" is accepted too, where the granularity of
#    the time span permits this. Parsing is generally locale-independent, non-English names for
#    the time units are not accepted.
#
#    Examples for valid time span specifications:
#
#        2 h
#        2hours
#        48hr
#        1y 12month
#        55s500ms
#        300ms20s 5day
#
systemd-run \
--user \
--on-active=30s \
--description="task to capture images" \
--timer-property=AccuracySec=100ms \
/bin/touch /tmp/foo1
#
#
# calendar time
# see http://manpages.ubuntu.com/manpages/bionic/man7/systemd.time.7.html 
# Using IANA format is recommended over local timezone names
# '2021-03-11 11:09'
#
#    Note that timestamps displayed by remote systems with a non-matching timezone are usually
#    not parsable locally, as the timezone component is not understood (unless it happens to be
#    "UTC").
#
#    Timestamps may also be specified with microsecond granularity. The sub-second remainder is
#    expected separated by a full stop from the seconds component. Example:
#
#        2014-03-25 03:59:56.654563
# 
systemd-run \
--user \
--on-calendar="2021-03-11 11:09:01" \
--description="task to capture images" \
--timer-property=AccuracySec=100ms \
/bin/touch /tmp/foo4_11am