import { useEffect, useRef, useState } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { Container, Row, Col, Button, Modal } from 'react-bootstrap';
import TreeNav from './components/TreeNav';
import GraphPanel from './components/GraphPanel';
import { useTelemetry } from './hooks/useTelemetry';
import { changeConfig, signalBufferEnable } from './store/actions';
import type { NavScheme, RootState, Sname } from './types/telemetry';
import './styles/app.css';
import './styles/fontello.css';
import 'bootstrap/dist/css/bootstrap.min.css';

export default function App() {
  const dispatch = useDispatch();
  const signals = useSelector((state: RootState) => state.signals);
  const containerRef = useRef<HTMLDivElement | null>(null);

  const [navScheme, setNavScheme] = useState<NavScheme>([]);
  const [listGraph, setListGraph] = useState<Sname[][]>([[]]);
  const [isShowConfig, setIsShowConfig] = useState(false);
  const [isDarkThemeConfig, setIsDarkThemeConfig] = useState(false);
  const [isCollapseNav, setIsCollapseNav] = useState(false);

  useTelemetry(setNavScheme);

  useEffect(() => {
    if (document.body) {
      document.body.style.overflow = 'hidden';
      document.body.style.touchAction = 'none';
    }
  }, []);

  useEffect(() => {
    const node = containerRef.current;
    if (!node) {
      return;
    }

    const onWheel = (event: WheelEvent) => {
      event.preventDefault();
    };

    node.addEventListener('wheel', onWheel, false);
    return () => node.removeEventListener('wheel', onWheel, false);
  }, []);

  const handleAddGraph = () => {
    setListGraph((prev) => [...prev, []]);
  };

  const handleCloseGraph = (iGraph: number) => {
    if (iGraph < listGraph.length) {
      setListGraph((prev) => {
        const next = [...prev];
        next.splice(iGraph, 1);
        return next;
      });
    } else {
      console.log('app::handleCloseGraph error (iGraph < listGraph.length)');
    }
  };

  const addSignalToGraph = (iGraph: number, sname: Sname) => {
    if (iGraph >= listGraph.length || !sname || !signals[sname]) {
      return;
    }
    if (listGraph[iGraph].includes(sname)) {
      return;
    }
    dispatch(signalBufferEnable(sname, true));
    setListGraph((prev) => {
      const next = [...prev];
      next[iGraph] = [...next[iGraph], sname];
      return next;
    });
  };

  const removeSignalFromGraph = (iGraph: number, sname: Sname) => {
    if (iGraph >= listGraph.length || !sname) {
      return;
    }
    setListGraph((prev) => {
      const next = [...prev];
      next[iGraph] = next[iGraph].filter((it) => it !== sname);
      return next;
    });
  };

  const handleAddSignalOnGraph = (sname: Sname) => {
    addSignalToGraph(0, sname);
  };

  const handleChangeConfig = () => {
    const config = {
      backgroundColor: isDarkThemeConfig ? 'white' : 'black',
    };
    dispatch(changeConfig(config));
    setIsDarkThemeConfig((prev) => !prev);
  };

  const clientHeight = document.documentElement ? document.documentElement.clientHeight : 300;

  const buttonStyle: React.CSSProperties = {
    fontSize: '16pt',
    marginRight: '5px',
    marginLeft: '5px',
    marginBottom: '5px',
  };

  return (
    <div ref={containerRef}>
      <Container className="col app-container" style={{ overflow: 'auto', height: clientHeight }}>
        <Row
          noGutters
          className="m-1 p-2"
          style={{ border: '1px solid #dbdbdb', borderRadius: '5px' }}
        >
          {!isCollapseNav ? (
            <Col className="col-auto">
              <Button className="icon-cog" style={buttonStyle} onClick={() => setIsShowConfig(true)} />
              <Button className="icon-doc" style={buttonStyle} onClick={handleAddGraph} />
              <TreeNav scheme={navScheme} onDoubleClick={handleAddSignalOnGraph} />
            </Col>
          ) : null}
          <Col className="col-auto">
            <Button
              style={{
                paddingLeft: '0px',
                paddingRight: '0px',
                width: '25px',
                ...buttonStyle,
              }}
              variant="info"
              onClick={() => setIsCollapseNav((prev) => !prev)}
            >
              {isCollapseNav ? String.fromCharCode(187) : String.fromCharCode(171)}
            </Button>
          </Col>
          <Col className="col">
            <GraphPanel
              listGraph={listGraph}
              onCloseGraph={handleCloseGraph}
              onAddSignal={addSignalToGraph}
              onRemoveSignal={removeSignalFromGraph}
            />
          </Col>
        </Row>
      </Container>

      <Modal show={isShowConfig} onHide={() => setIsShowConfig(false)} style={{ zIndex: 1e5 }}>
        <Modal.Header closeButton>
          <Modal.Title>Settings</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <input type="checkbox" checked={isDarkThemeConfig} onChange={handleChangeConfig} /> dark theme
        </Modal.Body>
      </Modal>
    </div>
  );
}
