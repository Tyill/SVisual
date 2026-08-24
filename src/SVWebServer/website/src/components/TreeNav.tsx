import { useEffect, useState } from 'react';
import '../styles/treeNav.css';
import '../styles/fontello.css';
import type { NavScheme, NavSchemeItem } from '../types/telemetry';

type TreeNavProps = {
  scheme: NavScheme;
  onDoubleClick: (sname: string) => void;
};

export default function TreeNav({ scheme, onDoubleClick }: TreeNavProps) {
  const [localScheme, setLocalScheme] = useState(scheme);

  useEffect(() => {
    setLocalScheme(scheme);
  }, [scheme]);

  const toggleSubmenu = (submenu: string) => {
    setLocalScheme((prev) =>
      prev.map((item) =>
        item.submenu === submenu ? { ...item, isShow: !item.isShow } : item,
      ),
    );
  };

  const renderSubmenu = (
    level: number,
    submenu: string,
    obj: string | NavSchemeItem,
    outObjList: Record<string, JSX.Element[]>,
    outMenuList: Record<string, JSX.Element>,
  ) => {
    if (typeof obj === 'string') {
      outObjList[submenu].push(
        <section
          key={level + '.' + obj}
          className="treeNav-row"
          draggable
          title="Double-click to add to graph"
          style={{ paddingLeft: 10 + level * 5, cursor: 'pointer' }}
          onDragStart={(e) => e.dataTransfer.setData('text', submenu + obj)}
          onDoubleClick={() => onDoubleClick(obj + submenu.slice(0, -1))}
        >
          {obj}
        </section>,
      );
    } else {
      const nextSubmenu = submenu + obj.submenu + '.';
      outObjList[nextSubmenu] = [];
      outMenuList[nextSubmenu] = (
        <section
          key={level + '.' + obj.submenu}
          className="treeNav-head"
          style={{
            paddingLeft: 10 + level * 5,
            backgroundColor: obj.isActive ? 'silver' : 'rgb(200, 20, 20)',
          }}
          onClick={() => toggleSubmenu(obj.submenu)}
        >
          {obj.submenu}
          <span className={obj.isShow ? 'icon-down-dir' : 'icon-left-dir'} />
        </section>
      );
      if (obj.isShow) {
        for (const o of obj.items) {
          renderSubmenu(level + 1, nextSubmenu, o, outObjList, outMenuList);
        }
      }
    }
  };

  const objList: Record<string, JSX.Element[]> = {};
  const menuList: Record<string, JSX.Element> = {};

  for (const obj of localScheme) {
    renderSubmenu(0, '', obj, objList, menuList);
  }

  const clientHeight = document.documentElement ? document.documentElement.clientHeight : 300;
  const renderObj: JSX.Element[] = [];

  for (const key in menuList) {
    renderObj.push(menuList[key]);
    if (objList[key].length > 0) {
      renderObj.push(
        <div key={key} style={{ overflow: 'auto', maxHeight: clientHeight * 0.8 + 'px' }}>
          {objList[key]}
        </div>,
      );
    }
  }

  return (
    <div style={{ borderRadius: '3px', overflow: 'auto', minWidth: '160px' }}>{renderObj}</div>
  );
}
