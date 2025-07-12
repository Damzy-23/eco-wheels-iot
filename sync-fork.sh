#!/bin/bash

# Sync Fork Script for Eco-Wheels IoT e-Bike Monitoring System
# This script helps synchronize a forked repository with the upstream

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    print_error "Not in a git repository. Please run this script from the project root."
    exit 1
fi

# Check if upstream remote exists
if ! git remote get-url upstream > /dev/null 2>&1; then
    print_warning "Upstream remote not found. Adding upstream remote..."
    echo "Please enter the upstream repository URL:"
    read -r upstream_url
    git remote add upstream "$upstream_url"
fi

print_status "Fetching latest changes from upstream..."
git fetch upstream

print_status "Checking current branch..."
current_branch=$(git branch --show-current)
print_status "Current branch: $current_branch"

print_status "Merging upstream changes..."
git merge upstream/main

print_status "Pushing changes to origin..."
git push origin "$current_branch"

print_status "Sync completed successfully!"
print_status "Your fork is now up to date with the upstream repository." 