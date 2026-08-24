import { useMemo, useRef } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { Container, Row, Col } from 'react-bootstrap';
import Graph from './Graph';
import { signalBufferEnable } from '../store/actions';
import type { RootState, Sname } from '../types/telemetry';

type GraphPanelProps = {
  listGraph: Sname[][];
  onCloseGraph: (iGraph: number) => void;
  onAddSignal: (iGraph: number, sname: Sname) => void;
  onRemoveSignal: (iGraph: number, sname: Sname) => void;
};

export default function GraphPanel({
  listGraph,
  onCloseGraph,
  onAddSignal,
  onRemoveSignal,
}: GraphPanelProps) {
  const dispatch = useDispatch();
  const signals = useSelector((state: RootState) => state.signals);
  const config = useSelector((state: RootState) => state.config);
  const dataParams = useSelector((state: RootState) => state.dataParams);
  const zIndexGraphRef = useRef(1);

  const handleAddSignal = (iGraph: number, sname: Sname) => {
    if (!sname || !signals[sname]) {
      return;
    }
    onAddSignal(iGraph, sname);
  };

  const handleDelSignal = (iGraph: number, sname: Sname) => {
    if (iGraph >= listGraph.length || !sname) {
      return;
    }

    let isOtherGraph = false;
    for (let i = 0; i < listGraph.length; ++i) {
      if (i === iGraph) {
        continue;
      }
      if (listGraph[i].includes(sname)) {
        isOtherGraph = true;
        break;
      }
    }

    if (!isOtherGraph) {
      dispatch(signalBufferEnable(sname, false));
    }

    onRemoveSignal(iGraph, sname);
  };

  const handleCloseGraph = (iGraph: number) => {
    if (iGraph < listGraph.length) {
      for (const sname of listGraph[iGraph]) {
        let isOtherGraph = false;
        for (let i = 0; i < listGraph.length; ++i) {
          if (i === iGraph) {
            continue;
          }
          if (listGraph[i].includes(sname)) {
            isOtherGraph = true;
            break;
          }
        }

        if (!isOtherGraph) {
          dispatch(signalBufferEnable(sname, false));
        }
      }

      onCloseGraph(iGraph);
    }
  };

  const graphSignalsList = useMemo(
    () =>
      listGraph.map((graphSnames) => {
        const graphSignals: RootState['signals'] = {};
        for (const s of graphSnames) {
          if (signals[s]) {
            graphSignals[s] = signals[s];
          }
        }
        return graphSignals;
      }),
    [listGraph, signals],
  );

  const sz = listGraph.length;
  const objList = [];

  for (let i = 0; i < sz; ++i) {
    const graphSignals = graphSignalsList[i];

    const xs = 12;
    const md = sz === 1 ? 12 : 6;
    const xl = sz === 1 ? 12 : sz === 2 ? 6 : 4;

    objList.push(
      <Col key={i} className={`pb-2 col-${xs} col-md-${md} col-xl-${xl} `}>
        <Graph
          iGraph={i}
          zIndex={zIndexGraphRef}
          dataParams={dataParams}
          signals={graphSignals}
          signalOrder={listGraph[i].filter((s) => graphSignals[s])}
          backgroundColor={config.backgroundColor}
          onAddSignal={handleAddSignal}
          onDelSignal={handleDelSignal}
          onCloseGraph={handleCloseGraph}
        />
      </Col>,
    );
  }

  return (
    <Container bsPrefix="fluid">
      <Row noGutters>{objList}</Row>
    </Container>
  );
}
