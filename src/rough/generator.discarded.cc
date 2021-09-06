void curveToBezier(); // from points-on-curve/lib/curve-to-bezier.js
void pointsOnBezierCurves(); // from points-on-curve
void pointsOnPath(); // from points-on-path

  Drawable path(const std::string& d, std::optional<Options> options) {
    const auto o = this->_o(options);
    std::vector<OpSet> paths;
    if (!d) {
      return this->_d(ShapeType::path, paths, o);
    }
    d = (d || '').replace(/\n/g, ' ').replace(/(-\s)/g, '-').replace('/(\s\s)/g', ' ');

    const auto hasFill = o.fill && o.fill != 'transparent' && o.fill != NOS;
    const auto hasStroke = o.stroke != NOS;
    const auto simplified = !!(o.simplification && (o.simplification < 1));
    const auto distance = simplified ? (4 - 4 * (o.simplification!)) : ((1 + o.roughness) / 2);
    const auto sets = pointsOnPath(d, 1, distance);

    if (hasFill) {
      if (o.combineNestedSvgPaths) {
        std::vector<Point>  combined;
        sets.forEach((set) => combined.push(...set));
        if (o.fillStyle == FillStyle::solid) {
          paths.push(solidFillPolygon(combined, o));
        } else {
          paths.push(patternFillPolygon(combined, o));
        }
      } else {
        sets.forEach((polyPoints) => {
          if (o.fillStyle == FillStyle::solid) {
            paths.push(solidFillPolygon(polyPoints, o));
          } else {
            paths.push(patternFillPolygon(polyPoints, o));
          }
        });
      }
    }
    if (hasStroke) {
      if (simplified) {
        sets.forEach((set) => {
          paths.push(linearPath(set, false, o));
        });
      } else {
        paths.push(svgPath(d, o));
      }
    }

    return this->_d(ShapeType::path, paths, o);
  }







  
  

  std::vector<PathInfo> toPaths(Drawable drawable) {
    const auto& sets = drawable.sets;
    const auto o = drawable.options.value_or(this->defaultOptions);
    std::vector<PathInfo> paths;
    for (const auto& drawing: sets) {
      std::optional<PathInfo> path;

      switch (drawing.type) {
        case OpSetType::path:
          path = {
            d: this->opsToPath(drawing),
            stroke: o.stroke,
            strokeWidth: o.strokeWidth,
            fill: NOS,
          };
          break;
        case OpSetType::fillPath:
          path = {
            d: this->opsToPath(drawing),
            stroke: NOS,
            strokeWidth: 0,
            fill: o.fill || NOS,
          };
          break;
        case OpSetType::fillSketch:
          path = this->fillSketch(drawing, o);
          break;
      }
      if (path) {
        paths.emplace_back(*path);
      }
    }
    return paths;
  }

  private PathInfo fillSketch(OpSet drawing, Options o) {
    auto fweight = o.fillWeight;
    if (fweight < 0) {
      fweight = o.strokeWidth / 2;
    }
    return {
      d: this->opsToPath(drawing),
      stroke: o.fill || NOS,
      strokeWidth: fweight,
      fill: NOS,
    };
  }

  Drawable curve(std::vector<Point> points, const Options& options={}) {
    auto o = options;
    std::vector<OpSet>  paths;
    const auto outline = ::curve(points, o);
    if (o.fill && *o.fill != NOS && points.size() >= 3) {
      const auto bcurve = curveToBezier(points);
      const auto polyPoints = pointsOnBezierCurves(bcurve, 10, (1 + o.roughness) / 2);
      if (o.fillStyle == FillStyle::solid) {
        paths.emplace_back(solidFillPolygon(polyPoints, o));
      } else {
        paths.emplace_back(patternFillPolygon(polyPoints, o));
      }
    }
    if (o.stroke != NOS) {
      paths.emplace_back(outline);
    }
    return this->_d(ShapeType::curve, paths, o);
  }