#!/bin/bash

min_size=10000
max_size=100000
step_size=10000
out_file="output.txt"
plot_file="plot.gnu"
algorithms=()

usage() {
    echo "Usage: $0 -i <min_size> -x <max_size> -s <step_size> -o <out_filename> -g <plot_script_filename> [bubble merge quick1 quick3 quickM]"
    exit 1
}

while getopts ":i:x:s:o:g:" opt; do
    case "$opt" in
        i) min_size=$OPTARG ;;
        x) max_size=$OPTARG ;;
        s) step_size=$OPTARG ;;
        o) out_file=$OPTARG ;;
        g) plot_file=$OPTARG ;;
        *) usage ;;
    esac
done

shift $((OPTIND - 1))

if [ $# -eq 0 ]; then
    algorithms=(bubble merge quick1 quick3 quickM)
else
    algorithms=("$@")
fi

for alg in "${algorithms[@]}"; do
    case "$alg" in
        bubble|merge|quick1|quick3|quickM) ;;
        *)
            echo "Unknown sort: $alg"
            exit 1
            ;;
    esac
done

: > "$out_file"

tmp_dir=$(mktemp -d)
trap 'rm -rf "$tmp_dir"' EXIT

for ((n=min_size; n<=max_size; n+=step_size)); do
    input_file="$tmp_dir/input_$n.txt"

    ./assgH02 --generate "$n" "$input_file" || exit 1

    results=$(./assgH02 "$input_file" "${algorithms[@]}") || exit 1

    line="$n"

    for alg in "${algorithms[@]}"; do
        result=$(printf '%s\n' "$results" |
            awk -v a="$alg" '$1==a {print $2, $3}')

        line="$line $result"
    done

    echo "$line" >> "$out_file"
done

plot_pdf="${plot_file%.*}.pdf"

{
    echo 'set terminal pdfcairo font "Helvetica,12" size 5in, 3in'
    echo "set output \"$plot_pdf\""
    echo 'set xlabel "Number of inputs"'
    echo 'set ylabel "Number of Comparison"'

    plot_cmd=""
    column=2

    for alg in "${algorithms[@]}"; do
        plot_cmd="$plot_cmd\"$out_file\" using 1:$column with linespoints title \"$alg\", "
        column=$((column + 2))
    done

    plot_cmd=${plot_cmd%, }
    echo "plot $plot_cmd"

    echo 'set ylabel "Computation time"'

    plot_cmd=""
    column=3

    for alg in "${algorithms[@]}"; do
        plot_cmd="$plot_cmd\"$out_file\" using 1:$column with linespoints title \"$alg\", "
        column=$((column + 2))
    done

    plot_cmd=${plot_cmd%, }
    echo "plot $plot_cmd"

    echo 'set output'
} > "$plot_file"

echo "Results written to $out_file"
echo "Gnuplot script written to $plot_file"
gnuplot "$plot_file"
