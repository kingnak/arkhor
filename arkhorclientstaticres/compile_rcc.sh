#!/bin/bash
rcc -binary "$1/client_resources.qrc" -o client_resources.rcc
cp client_resources.rcc ../arkhorclient/
