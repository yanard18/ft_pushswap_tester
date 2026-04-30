#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PUSH_SWAP="$ROOT_DIR/push_swap"
CHECKER="$ROOT_DIR/checker_linux"

./run

# Check if binaries exist
if [ ! -f "$CHECKER" ]; then
    echo -e "${RED}Error: checker_linux not found at $CHECKER.${NC}"
    exit 1
fi

echo -e "${BLUE}=== Starting ft_pushswap Full Evaluation ===${NC}"

# 1. Compilation
echo -ne "Compilation: "
make -C "$ROOT_DIR" re > /dev/null 2>&1
if [ $? -eq 0 ] && [ -f "$PUSH_SWAP" ]; then
    echo -e "${GREEN}PASS${NC}"
else
    echo -e "${RED}FAIL${NC}"
    exit 1
fi

# 2. Error Management
echo -e "\n${YELLOW}[Error Management]${NC}"

test_error() {
    local label=$1
    local args=$2
    echo -ne "$label: "
    res=$($PUSH_SWAP $args 2>&1)
    if [ "$res" == "Error" ]; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL (Got: '$res')${NC}"
    fi
}

test_error "Non-numeric" "a 1 2"
test_error "Duplicates" "1 1 2"
test_error "MAXINT+" "2147483648 1 2"
echo -ne "Empty Input: "
res=$($PUSH_SWAP)
if [ -z "$res" ]; then echo -e "${GREEN}PASS${NC}"; else echo -e "${RED}FAIL${NC}"; fi

# 3. Identity Test
echo -e "\n${YELLOW}[Identity Test - Sorted Inputs]${NC}"
test_identity() {
    echo -ne "Input '$1': "
    res=$($PUSH_SWAP $1)
    if [ -z "$res" ]; then echo -e "${GREEN}PASS${NC}"; else echo -e "${RED}FAIL${NC}"; fi
}
test_identity "42"
test_identity "2 3"
test_identity "0 1 2 3"
test_identity "0 1 2 3 4 5 6 7 8 9"

# 4. Small/Medium Inputs
echo -e "\n${YELLOW}[Small/Medium Performance]${NC}"
test_perf() {
    local size=$1
    local limit=$2
    local args=$3
    echo -ne "$size numbers ($args): "
    ops=$($PUSH_SWAP $args | wc -l)
    check=$($PUSH_SWAP $args | "$CHECKER" $args)
    if [ "$check" == "OK" ] && [ "$ops" -le "$limit" ]; then
        echo -e "${GREEN}PASS ($ops ops)${NC}"
    else
        echo -e "${RED}FAIL ($check, $ops ops, limit $limit)${NC}"
    fi
}
test_perf "3" 3 "2 1 0"
test_perf "3" 3 "0 2 1"
test_perf "5" 12 "1 5 2 4 3"
test_perf "5" 12 "5 1 4 2 3"

# 5. Benchmark & Disorder
echo -e "\n${YELLOW}[Benchmark & Disorder]${NC}"
echo -ne "Disorder 100% (5 4 3 2 1): "
res=$($PUSH_SWAP --bench 5 4 3 2 1 2>&1 | grep "disorder: 100.00%")
if [ ! -z "$res" ]; then echo -e "${GREEN}PASS${NC}"; else echo -e "${RED}FAIL${NC}"; fi
echo -ne "Disorder 0% (1 2 3 4 5): "
res=$($PUSH_SWAP --bench 1 2 3 4 5 2>&1 | grep "disorder: 0.00%")
if [ ! -z "$res" ]; then echo -e "${GREEN}PASS${NC}"; else echo -e "${RED}FAIL${NC}"; fi

# 6. Large Inputs
echo -e "\n${YELLOW}[Large Inputs Performance]${NC}"
run_large() {
    local size=$1
    local range=$2
    local excellent=$3
    local good=$4
    local pass=$5
    
    ARG=$(shuf -i 1-$range -n $size | tr '\n' ' ')
    ops=$($PUSH_SWAP $ARG | wc -l)
    check=$($PUSH_SWAP $ARG | "$CHECKER" $ARG)
    
    echo -ne "$size elements: "
    if [ "$check" != "OK" ]; then
        echo -e "${RED}KO (Check failed)${NC}"
    elif [ "$ops" -le "$excellent" ]; then
        echo -e "${GREEN}EXCELLENT ($ops ops)${NC}"
    elif [ "$ops" -le "$good" ]; then
        echo -e "${BLUE}GOOD ($ops ops)${NC}"
    elif [ "$ops" -le "$pass" ]; then
        echo -e "${YELLOW}PASS ($ops ops)${NC}"
    else
        echo -e "${RED}FAIL ($ops ops, limit $pass)${NC}"
    fi
}

run_large 100 500 700 1500 2000
run_large 100 500 700 1500 2000
run_large 500 1000 5500 8000 12000
run_large 500 1000 5500 8000 12000

echo -e "\n${BLUE}=== Evaluation Script Finished ===${NC}"
