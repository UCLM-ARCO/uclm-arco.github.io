# Source this file to setup the environment

# Define some colors and logging functions
GREEN="\e[32m"
NORM="\e[0m"

function _info() {
    local msg=$1
    echo -e "${GREEN}${msg}${NORM}"
}

# Create a new section, and populate with default templates
function ad-create-section() {
    local name=$1
    if [ -z "$name" ]; then
        echo "usage: ${FUNCNAME[0]} <name>"
        return
    fi

    if [ -d "$name" ]; then
        echo "Error: that section ($name) already exists"
        return
    fi

    echo "Please, provide the type of section."
    echo "Choose from [project, internal-project, recipes, ...]"
    echo -n "- "
    read prj_type

    echo "And a brief description? (intro when finish)"
    echo -n "- "
    read prj_desc
    prj_desc=$(echo "$prj_desc" | sed -e 's/[\/&]/\\&/g')

    echo "Creating section dirs..."
    mkdir -p "$name"/{content,static/images}

    echo "Populating with initial content..."
    local idx="$name/content/_index.md"
    local prj_icon="$name/images/$name-logo.png"
    local prj_title=$(echo $name | sed 's/./\U&/')
    cp templates/section_index.md "$idx"
    sed -i "s|__TYPE__|$prj_type|g" "$idx"
    sed -i "s|__TITLE__|$prj_title|g" "$idx"
    sed -i "s|__DESC__|$prj_desc|g" "$idx"
    sed -i "s|__ICON__|$prj_icon|g" "$idx"

    echo "Adding section to automated tools..."
    local mk_section="SECTIONS += $name"
    sed -i '/^# SECTIONS_VAR_END.*/ i '"$mk_section" Makefile

    echo -e "Done. \n"
    _info "- You can modify the given parameters in '$idx'"
    echo -e "- Also, don't forget to provide an icon ($prj_icon).\n"
}




